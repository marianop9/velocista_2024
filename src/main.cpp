#include <stdio.h>

#include "Arduino.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "motor.h"
#include "tcrt.h"
extern "C" {
#include "state_machine.h"
}
#include "pid_tuner.h"

#define SENSOR_COUNT 8
int sensorPins[SENSOR_COUNT] = {35, 32, 33, 25, 26, 27, 14, 12};
#define PIN_SENSOR_ENABLE GPIO_NUM_4
#define PIN_LED GPIO_NUM_19
#define PIN_BTN GPIO_NUM_21

#define PIN_MOTOR_L 22
#define PIN_MOTOR_R 23

// #define WIFI_SSID "Fibertel WiFi709 2.4GHz"
// #define WIFI_PWD "01429015004"
// #define WIFI_SSID "Fibertel WiFi272 2.4GHz"
// #define WIFI_PWD "00443299212"
// #define WIFI_SSID "Utn_Libre Max"
// #define WIFI_PWD ""
#define WIFI_SSID "Nano"
#define WIFI_PWD "mariano12"

extern "C" {
void app_main();
}

bool logError = false;

TaskHandle_t mainTask = NULL;
TaskHandle_t sensorTask = NULL;

PIDController pidController;

void tcrt_sensor_task(void *args) {
    struct tcrt_sensor_array_t *tcrt = tcrt_init(SENSOR_COUNT, sensorPins, PIN_SENSOR_ENABLE);

    while (1) {
        int32_t error = (int32_t)tcrt_calculate_error(tcrt);
        // tcrt_log_readings(tcrt);

        xTaskNotify(mainTask, error, eSetValueWithOverwrite);

        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}

void main_task(void *args) {    
    motorInit(PIN_MOTOR_L, PIN_MOTOR_R);
    state_machine_init(PIN_BTN, PIN_LED);

    int32_t receivedErr = 0;
    int motorDutyCycles[2] = {0};
    
    while (1) {
        if (state_machine_is_stopped()) {
            motorStop();
            continue;
        }

        // portMAX_DELAY blocks indefdently
        // si la espera de la notificacion da timeout no actualiza
        if (xTaskNotifyWait(0x00, ULONG_MAX, (uint32_t *)&receivedErr, 500/portTICK_PERIOD_MS) == pdFALSE)
            continue;

        pidController.update(receivedErr);

        int adjustment = pidController.getAdjustment();
        motorUpdate(adjustment);

        if (logError) {
            printf("error: %d\n", receivedErr);
            motorSimulateUpdate(adjustment, motorDutyCycles);
            printf("dutyL: %d \t dutyR: %d\n", motorDutyCycles[0], motorDutyCycles[1]);
        }
    }
}

void app_main() {
    initArduino();

    // vTaskDelay(pdMS_TO_TICKS(1000));
    pid_tuner_init(WIFI_SSID, WIFI_PWD, &pidController);

    xTaskCreatePinnedToCore(tcrt_sensor_task, "Sensor readings task", configMINIMAL_STACK_SIZE * 4,
                            NULL, 5, &sensorTask, 1);

    xTaskCreatePinnedToCore(main_task, "Motor update task", configMINIMAL_STACK_SIZE * 5, NULL, 5,
                            &mainTask, 0);
}

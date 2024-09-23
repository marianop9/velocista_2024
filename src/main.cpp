#include <stdio.h>

#include "Arduino.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "motor.h"
#include "tcrt.h"
extern "C" {
#include "state_machine.h"
}

#define SENSOR_COUNT 8
int sensorPins[SENSOR_COUNT] = {35, 32, 33, 25, 26, 27, 14, 12};
#define PIN_SENSOR_ENABLE GPIO_NUM_4
#define PIN_LED GPIO_NUM_19
#define PIN_BTN GPIO_NUM_21

#define PIN_MOTOR_L 22
#define PIN_MOTOR_R 23

// #define WIFI_SSID "Fibertel WiFi709 2.4GHz"
// #define WIFI_PWD "01429015004"
#define WIFI_SSID "Fibertel WiFi272 2.4GHz"
#define WIFI_PWD "00443299212"

extern "C" {
void app_main();
}

TaskHandle_t mainTask = NULL;
TaskHandle_t sensorTask = NULL;

void tcrt_sensor_task(void *args) {
    struct tcrt_sensor_array_t *tcrt = tcrt_init(SENSOR_COUNT, sensorPins, PIN_SENSOR_ENABLE);

    while (1) {
        int32_t error = (int32_t)tcrt_calculate_error(tcrt);

        xTaskNotify(mainTask, error, eSetValueWithOverwrite);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void main_task(void *args) {
    state_machine_init(PIN_BTN, PIN_LED);

    printf("starting loop....\n");

    int32_t receivedErr = 0;
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1500));

        if (state_machine_is_stopped()) continue;

        // portMAX_DELAY blocks indefdently
        xTaskNotifyWait(0x00, ULONG_MAX, (uint32_t *)&receivedErr, portMAX_DELAY);

        printf("error: %d\n", receivedErr);
        // tcrt_log_readings(tcrt);
    }
}

void app_main() {
    initArduino();

    // vTaskDelay(pdMS_TO_TICKS(1000));
    // pid_tuner_init(WIFI_SSID, WIFI_PWD);

    xTaskCreatePinnedToCore(tcrt_sensor_task, "Sensor readings task", configMINIMAL_STACK_SIZE * 4,
                            NULL, 5, &sensorTask, 1);

    xTaskCreatePinnedToCore(main_task, "Main task", configMINIMAL_STACK_SIZE * 5, NULL, 5,
                            &mainTask, 0);
}

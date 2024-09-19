#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "Arduino.h"

#include "tcrt.h"
#include "pid_tuner.h"

int sensorsEnablePin = 4;
int sensorPins[8] = {35, 32, 33,  25, 26, 27, 14, 12};

#define WIFI_SSID "Fibertel WiFi272 2.4GHz"
#define WIFI_PWD "00443299212"

extern "C" {
    void app_main();
}

void app_main() {
    initArduino();
    
    vTaskDelay(pdMS_TO_TICKS(1000));

    struct tcrt_sensor_array_t *tcrt = tcrt_init(8, sensorPins, sensorsEnablePin);

    pid_tuner_init(WIFI_SSID, WIFI_PWD);

    printf("starting loop....\n");
    while(1)
    {
        // printf("error: %d\n", tcrt_calculate_error(tcrt));
        tcrt_log_readings(tcrt);
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}
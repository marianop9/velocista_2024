#ifndef _PID_TUNER_H_
#define _PID_TUNER_H_

#include "Arduino.h"
#include "freertos/FreeRTOS.h"

void pid_tuner_init(const char *ssid, const char *pwd);

enum class PIDParam
{
    Proportional = 0,
    Integral,
    Derivative,
    InvalidParam
};

PIDParam PIDParamFromString(const char* str);

class PIDController
{
private:
    int P = 0, I = 0, D = 0;

    // PID params
    int kp = 1, ki = 2, kd = 3;

    int lastError = 0;

    int adjdustment = 0;

    SemaphoreHandle_t _mutex;

public:
    PIDController() {
        // semaphore is created in empty state. Must be given before it can be taken.
        _mutex = xSemaphoreCreateBinary();

        if (_mutex == NULL) {
            ESP_LOGE("PIDController", "failed to initalize mutex");
        } else {
            xSemaphoreGive(_mutex);
        }
    }

    void setParam(PIDParam param, int value);

    int getParam(PIDParam param);

    void update(int error);

    int getAdjustment()
    {
        return adjdustment;
    }
};

#endif // _PID_TUNER_H_
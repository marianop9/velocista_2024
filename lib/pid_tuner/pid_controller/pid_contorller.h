#ifndef _PID_CONTROLLER_H_
#define _PID_CONTROLLER_H_

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

enum class PIDParam { Proportional = 0, Integral, Derivative, InvalidParam };

PIDParam PIDParamFromString(const char* str);

class PIDController {
   private:
    int P = 0, I = 0, D = 0;

    // PID params
    int kp = 1, ki = 2, kd = 3;

    int lastError = 0;

    int adjdustment = 0;

    SemaphoreHandle_t _mutex;

   public:
    PIDController();

    void setParam(PIDParam param, int value);

    int getParam(PIDParam param) const;

    void update(int error);

    int getAdjustment() const { return adjdustment; }

    int getLastError() const { return lastError; }
};

#endif  // _PID_CONTROLLER_H_
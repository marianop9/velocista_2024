#include "pid_contorller.h"
#include "string.h"

static const char* TAG = "PIDController";

PIDParam PIDParamFromString(const char* str) {
    ESP_LOGI(TAG, "PID from string - got param %s", str);
    if (strcmp(str, "kp") == 0)
        return PIDParam::Proportional;
    else if (strcmp(str, "ki") == 0)
        return PIDParam::Integral;
    else if (strcmp(str, "kd") == 0)
        return PIDParam::Derivative;

    return PIDParam::InvalidParam;
}

// Default constructor
PIDController::PIDController() {
    // semaphore is created in empty state. Must be given before it can be
    // taken.
    _mutex = xSemaphoreCreateBinary();

    if (_mutex == NULL) {
        ESP_LOGE(TAG, "failed to initalize mutex");
    } else {
        xSemaphoreGive(_mutex);
    }
}

void PIDController::setParam(PIDParam param, int value) {
    if (!xSemaphoreTake(_mutex, pdMS_TO_TICKS(500))) {
        return;
    }

    switch (param) {
        case PIDParam::Proportional:
            kp = value;
            break;
        case PIDParam::Integral:
            ki = value;
            break;
        case PIDParam::Derivative:
            kd = value;
            break;
        default:
            break;
    }

    xSemaphoreGive(_mutex);
}

int PIDController::getParam(PIDParam param) const {
    ESP_LOGI(TAG, "getParam - retrieving lock...");

    if (!xSemaphoreTake(_mutex, pdMS_TO_TICKS(500))) {
        return -9;
    }

    int value = -1;
    switch (param) {
        case PIDParam::Proportional:
            value = kp;
            break;
        case PIDParam::Integral:
            value = ki;
            break;
        case PIDParam::Derivative:
            value = kd;
            break;
        default:
            break;
    }
    ESP_LOGI(TAG, "getParam - returning parameter value: %d", value);

    xSemaphoreGive(_mutex);
    return value;
}

int PIDController::update(int error) {
    P = error;
    I = I + error;
    D = error - lastError;

    lastError = error;

    adjustment = P * kp + I * ki + D * kd;  // calculate the correction
    return adjustment;
}
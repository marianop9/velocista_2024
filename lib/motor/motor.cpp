#include "motor.h"

#include "Arduino.h"

const int PWMFreq = 5000; /* 5 KHz */
const int PWMChannelLeft = 0;
const int PWMChannelRight = 2;
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (1 << PWMResolution) - 1;
const int baseSpeed = 700;

int dutyCycleL = baseSpeed;
int dutyCycleR = baseSpeed;

void motorInit(int leftMotorPin, int rightMotorPin)
{
    ledcSetup(PWMChannelLeft, PWMFreq, PWMResolution);
    ledcSetup(PWMChannelRight, PWMFreq, PWMResolution);

    ledcAttachPin(leftMotorPin, PWMChannelLeft);
    ledcAttachPin(rightMotorPin, PWMChannelRight);
}

void motorUpdate(int adjustment)
{
    dutyCycleL = baseSpeed - adjustment;
    dutyCycleR = baseSpeed + adjustment;

    // Si el ajuste hace que se supere el maximo o el minimo corrijo
    if (dutyCycleL < 0)
        dutyCycleL = 0;
    else if (dutyCycleL > MAX_DUTY_CYCLE)
        dutyCycleL = MAX_DUTY_CYCLE;

    if (dutyCycleR > MAX_DUTY_CYCLE)
        dutyCycleR = MAX_DUTY_CYCLE;
    else if (dutyCycleR < 0)
        dutyCycleR = 0;

    ledcWrite(PWMChannelLeft, dutyCycleL);
    ledcWrite(PWMChannelRight, dutyCycleR);
}

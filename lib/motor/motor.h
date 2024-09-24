#ifndef MOTOR_h
#define MOTOR_h

void motorInit(int leftMotor, int rightMotor);
void motorUpdate(int adjustment);
void motorSimulateUpdate(int adjustment, int values[2]);
void motorStop();

#endif
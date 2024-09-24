#ifndef _PID_TUNER_H_
#define _PID_TUNER_H_

#include "pid_controller/pid_controller.h"

void pid_tuner_init(const char *ssid, const char *pwd, PIDController *pidControllerInstance);

#endif // _PID_TUNER_H_
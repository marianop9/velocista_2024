void pid_tuner_init(const char *ssid, const char *pwd);

enum PIDParam
{
    Proportional = 0,
    Integral,
    Derivative,
};

class PIDController
{
private:
    int P = 0, I = 0, D = 0;

    // PID params
    int kp = 1, ki = 2, kd = 3;

    int lastError = 0;

    int adjdustment = 0;

    portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

public:
    void setParam(PIDParam param, int value);

    int getParam(PIDParam param);

    void update(int error);

    int getAdjustment()
    {
        return adjdustment;
    }
};
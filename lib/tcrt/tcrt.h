struct tcrt_sensor_array_t {
    int sensorCount;
    int* pins;
    int enablePin;
};

struct tcrt_sensor_array_t* tcrt_init(int sensorCount, int* pins, int enablePin);

int tcrt_calculate_error(struct tcrt_sensor_array_t* tcrt);

void tcrt_log_readings(struct tcrt_sensor_array_t* tcrt);

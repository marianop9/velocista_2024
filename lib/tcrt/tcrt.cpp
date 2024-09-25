#include "tcrt.h"
#include "Arduino.h"

int lastError;

struct tcrt_sensor_array_t* tcrt_init(int sensorCount, int* pins, int enablePin)
{
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, HIGH);
    for (int i = 0; i < sensorCount; i++)
    {
        pinMode(pins[i], INPUT);
    }

    struct tcrt_sensor_array_t *tcrt = (struct tcrt_sensor_array_t*)malloc(sizeof(struct tcrt_sensor_array_t));

    tcrt->sensorCount = sensorCount;
    tcrt->pins = pins;
    tcrt->enablePin = enablePin;

    return tcrt;
}

int tcrt_calculate_error(struct tcrt_sensor_array_t* tcrt) 
{
    // Kp: 450, 850
    //  int sensorWeights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};
    
    int sensorWeights[8] = {-9, -5, -3, -1, 1, 3, 5, 9};

    int error = 0;
    bool hasLine = false;
    for (int i = 0; i < tcrt->sensorCount; i++)
    {
        // solo suma los sensores activos (estado bajo)
        if(!digitalRead(tcrt->pins[i]))
        {
            error += sensorWeights[i];
            hasLine = true;
        }
    }

    if (!hasLine) {
        return lastError;
    }
    
    lastError = error;
    return error;    
}

void tcrt_log_readings(struct tcrt_sensor_array_t* tcrt)
{
    for (int i = 0; i < tcrt->sensorCount; i++)
    {
        printf("%d\t", i);        
    }
    printf("\n");
    
    for (int i = 0; i < tcrt->sensorCount; i++)
    {
        int value = digitalRead(tcrt->pins[i]);
        printf("%d\t", value);
    }
    printf("\n");
    printf("------------------------------------\n");
}
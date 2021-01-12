#ifndef TASK1___
#define TASK1___

#include "interconnectivity_milestone.h"


typedef struct StateVars {
    int sequence;
    int state;
    int sensorTotal;
    int sensorCount;
    int curTime;
} StateVars;



// adc handle - we'll read from it here
ADC_Handle adc;

// CB timer for 10ms, so we can get 90ms and 500ms periods
Timer_Handle timer1;

// state machine states
#define INIT                    1
#define WaitingForTime1         2
#define WaitingForTime2         3
#define WaitingForTime3         4

void Task1CB(char* topic, char* payload);

// state machine for sending data to our queue
int handle_data_state_machine(StateVars *smVars, int sensorValue, int timeInc);
void task1_timerCallback(Timer_Handle myHandle, int_fast16_t status);
void *task1Thread(void * arg0);
void createTask1();
void publishSensorAverages();
void publishAnalytics();
void init_timers();

#endif

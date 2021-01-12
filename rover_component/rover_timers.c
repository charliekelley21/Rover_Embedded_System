/*
 * rover_timers.c
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#include <rover_timers.h>

void setN(int count) {
    N = count;
}

int getN() {
    return N;
}

Timer_Handle timerInit()
{
    Timer_Params params;
    Timer_Params_init(&params);
    params.period = 100000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCB;

    return Timer_open(CONFIG_TIMER_0, &params);
}

void timerCB(Timer_Handle myHandle)
{
    if(!encoder_push_msg(TIME_ID))
    {
        dbgErrorRoutine(HARDWARE_TIMER_ERROR);
    }
    if (N == -1) {
        struct state_t temp = {0, 0.0, 0.0, 0};
        state_push(temp);
        N++;
    }
    if (N == 0) {
        //do nothing, wait for next movement command
    }
    else if (N == 1) {
        // push 0 speed to control for each motor
        wheel_t rotate_1 = {0, 4, 0, 0};
        wheel_t rotate_2 = {0, 5, 0, 0};
        wheel_t rotate_3 = {0, 6, 0, 0};
        control_push_wheel(rotate_1);
        control_push_wheel(rotate_2);
        control_push_wheel(rotate_3);
        N--;

    }
    else {
        N--;
    }
}

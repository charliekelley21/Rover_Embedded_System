/*
 * rover_timers.h
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#ifndef ROVER_TIMERS_H_
#define ROVER_TIMERS_H_

#include "rover_component.h"

#include <ti/drivers/Timer.h>
#include "encoder_queue.h"
#include <timers.h>
#include "debug.h"
#include "control_queue.h"
#include "json_parser.h"
#include "state_queue.h"

/* Driver configuration */
#include "ti_drivers_config.h"

int N;

void setN(int count);
int getN();
Timer_Handle timerInit();
void timerCB(Timer_Handle myHandle);

#endif /* ROVER_TIMERS_H_ */

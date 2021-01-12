/*
 * movement_thread.h
 *
 *  Created on: Oct 18, 2020
 *      Author: chuck
 */

#ifndef MOVEMENT_THREAD_H_
#define MOVEMENT_THREAD_H_

#include "rover_component.h"

#include <unistd.h>
#include <math.h>

#include <FreeRTOS.h>
#include <queue.h>
#include "ti_drivers_config.h"
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>

#include "rover_timers.h"
#include "movement_queue.h"
#include "control_queue.h"
#include "json_parser.h"
#include "event_group.h"

void create_movement_task();
void *movement_thread(void *arg0);
void move_forward(float dist_m);
void move_rotate(float theta_rad);

#endif /* MOVEMENT_THREAD_H_ */

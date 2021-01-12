/*
 * state_thread.h
 *
 *  Created on: Oct 28, 2020
 *      Author: chuck
 */

#ifndef STATE_THREAD_H_
#define STATE_THREAD_H_

#include "rover_component.h"

#include <unistd.h>
#include <math.h>

#include <ti/drivers/UART.h>
#include "uart_rover.h"
#include "movement_queue.h"
#include "state_queue.h"
#include "json_parser.h"
#include "debug.h"

#define ARM_FINISH_OBJ      10
#define ARM_FINISH_DROP     12
#define ARM_OUT_OF_REACH    13
#define ARM_DROP            14
#define RESTART_ID          15


#define ARM_BACK        ((float) 0.0)
#define ARM_ROTATE      ((float) 2.094)

enum rover_states {
    INIT,
    WAITING,
    MOVE_FRONT,
    MOVE_OBJ,
    MOVE_DROPOFF,
    ARM_ORIENT,
    ARM_WAIT,
    ARM_ADJUST,
    ARM_DROPPED,
    POST_ODOMETRY,
    FINISH
} rover_states;

enum project_states {
    EMPTY,
    LOADED,
    DROPPED,
    COMPLETED
} project_states;

void create_state_task();
void *state_thread(void *arg0);
int fsm_rover(struct state_t state_msg);
void publish_movement(struct move_t odom, float arm_rotation);
void move_rover(struct move_t move, int odom);
void arm_adjust();

#endif /* STATE_THREAD_H_ */

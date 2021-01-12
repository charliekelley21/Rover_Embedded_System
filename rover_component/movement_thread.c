/*
 * movement_thread.c
 *
 *  Created on: Oct 18, 2020
 *      Author: chuck
 */

#include "movement_thread.h"

void create_movement_task() {
    // create simple code structure thread
    int retc;
    pthread_t           thread;
    pthread_attr_t      attrs;
    struct sched_param  priParam;

    /* Initialize the attributes structure with default values */
    pthread_attr_init(&attrs);

    /* Set priority, detach state, and stack size attributes */
    priParam.sched_priority = 2;
    retc = pthread_attr_setschedparam(&attrs, &priParam);
    retc |= pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_DETACHED);
    retc |= pthread_attr_setstacksize(&attrs, THREADSTACKSIZE);
    if (retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);

    retc = pthread_create(&thread, &attrs, movement_thread, NULL);
    if (retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);
}

void *movement_thread(void *arg0) {
    setN(0);
    struct move_t temp;

    while (event_connected() == -1) {
        ;
    }

    while(1) { // read from queue
        if (getN() == 0) {
            if (movement_pop(&temp)) {
                if (temp.sequence == -1) {
                    setN(-1);
                }
                if (temp.distance == 0.0 && temp.theta == 0.0) {
                    ;
                }
                else if (temp.distance == 0.0) {
                    move_rotate(temp.theta);
                }
                else {
                    move_forward(temp.distance);
                }
            }
        }
    }
}

void move_forward(float dist_m) {
    //setN based on distance passed in
    int wheel_2_direction = (dist_m >= 0) ? 0 : 1;
    int wheel_3_direction = (dist_m >= 0) ? 1 : 0;
    setN(dist_m * 10.);
    setN( ( int ) ( fabs(dist_m) * 111.219 + 1.0) );
    wheel_t forward_2 = {wheel_2_direction, 5, 50, 0};
    wheel_t forward_3 = {wheel_3_direction, 6, 50, 0};
    control_push_wheel(forward_2);
    control_push_wheel(forward_3);
}

void move_rotate(float theta_rad) {
    //setN based on fabs(theta) passed in
    setN( ( int ) ( fabs(theta_rad) * 33.0766 + 1) );
    int direction = (theta_rad < 0) ? 1 : 0;
    wheel_t rotate_1 = {direction, 4, 30, 0};
    wheel_t rotate_2 = {direction, 5, 30, 0};
    wheel_t rotate_3 = {direction, 6, 30, 0};
    control_push_wheel(rotate_1);
    control_push_wheel(rotate_2);
    control_push_wheel(rotate_3);
}

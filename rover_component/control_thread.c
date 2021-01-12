/*
 * control_thread.c
 *
 *  Created on: Oct 7, 2020
 *      Author: chuck
 */

#include "control_thread.h"

void create_control_task()
{
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

    retc = pthread_create(&thread, &attrs, control_thread, NULL);
    if (retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);
}

void *control_thread(void *arg0) {
    motor_driver_init();
    encoder_push_msg(START_ID);

    struct controller_t control_1 = {0, 128, FORWARD, 0, 0, 0, 0};
    struct controller_t control_2 = {0, 129, FORWARD, 0, 0, 0, 0};
    struct controller_t control_3 = {0, 130, FORWARD, 0, 0, 0, 0};

    struct wheel_t temp;

    float Kp = 0.35;
    float Ki = 0.15;


    while (event_connected() == -1) {
        ;
    }

    dbgOutputLoc(CONTROL_TASK);

    while (1) {
        if (control_pop(&temp)) {
//            LOG_INFO("Wheel Command: speed = %d", temp.pos);
            switch(temp.index) {
            case MOTOR_1:
                // wheel one read, pid
                pi_algorithm(&control_1, temp, Kp, Ki);
                break;
            case MOTOR_2:
                // wheel two read, pid
                pi_algorithm(&control_2, temp, Kp, Ki);
                break;
            case MOTOR_3:
                // wheel three read, pid
                pi_algorithm(&control_3, temp, Kp, Ki);
                break;
            case 4:
                //wheel 1 update
                control_1.direction = temp.sequence;
                control_1.desired_speed = temp.pos;
                break;
            case 5:
                //wheel 2 update
                control_2.direction = temp.sequence;
                control_2.desired_speed = temp.pos;
                break;
            case 6:
                //wheel 3 update
                control_3.direction = temp.sequence;
                control_3.desired_speed = temp.pos;
                break;
            }

        }
    }
}


void pi_algorithm(struct controller_t* ref, struct wheel_t wheel, float Kp, float Ki) {
    dbgOutputLoc(CONTROL_PID);

    ref->sequence++;

    // encoder sanity check
    int ticks = abs(wheel.pos - wheel.prev_pos);
    if ((ticks >= 250) || (ticks < 0) ) {
        dbgErrorRoutine(ERROR_CONTROL);
    }

    ref->error = ref->desired_speed - ticks;
    ref->integral += ref->error;

    float rewrite = Kp * ref->error + ref->integral * Ki;   // ticks/100ms -> UART output
    int rewrite_i = round(rewrite);

    ref->current_speed += rewrite_i;
    if (ref->current_speed > 127)
        ref->current_speed = 127;
    else if (ref->current_speed < 0)
        ref->current_speed = 0;

    wheel_write(wheel.index, ref->direction, ref->current_speed);          // write speed to uart

    static struct msgQueue msg;
    msg.event = CONTROL_MSG;

    msg.data_len = encode_control(msg.payload, *ref);
    if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
        broker_push(&msg);
}





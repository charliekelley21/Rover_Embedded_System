/*
 * encoder_thread.c
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#include "encoder_thread.h"

void create_encoder_task()
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

    retc = pthread_create(&thread, &attrs, encoder_thread, NULL);
    if (retc != 0)
        dbgErrorRoutine(ERROR_TASK_CREATE);
}

void *encoder_thread(void *arg0) {

    GPIO_setConfig(slave_select_1, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(slave_select_2, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH);
    GPIO_setConfig(slave_select_3, GPIO_CFG_OUTPUT | GPIO_CFG_OUT_HIGH);

    init_SPI();

    int ret = init_encoder(slave_select_1);
    ret = init_encoder(slave_select_2);
    ret = init_encoder(slave_select_3);

    if (!ret) {
        dbgErrorRoutine(ERROR_SPI);
    }

    Timer_Handle timer0 = timerInit();
    if (timer0 == NULL) {
        dbgErrorRoutine(ERROR_TIMER_INIT);
    }

    int encoder_case;

    struct wheel_t wheel1 = {0, MOTOR_1, 0, 0};
    struct wheel_t wheel2 = {0, MOTOR_2, 0, 0};
    struct wheel_t wheel3 = {0, MOTOR_3, 0, 0};

    while (event_connected() == -1) {
        ;
    }

    dbgOutputLoc(ENCODER_TASK);

    while (1) {
        if (encoder_pop(&encoder_case)) {
            if (encoder_case == TIME_ID) {

                rover_position(&wheel1, &wheel2, &wheel3);

                control_push_wheel(wheel1);
                control_push_wheel(wheel2);
                control_push_wheel(wheel3);
            }
            else if (encoder_case == START_ID) {
                if (Timer_start(timer0) == Timer_STATUS_ERROR) {
                    dbgErrorRoutine(ERROR_TIMER_INIT);
                }
            }
            else {
                dbgErrorRoutine(ERROR_SPI);
            }
        }
    }
}

void rover_position(struct wheel_t* wheel1, struct wheel_t* wheel2, struct wheel_t* wheel3) {
    dbgOutputLoc(GET_SPI_READING);
    static struct msgQueue msg;
    static int encoder_sequence = 0;
    msg.event = ENCODER_MSG;

    wheel1->prev_pos = wheel1->pos;
    wheel2->prev_pos = wheel2->pos;
    wheel3->prev_pos = wheel3->pos;

    wheel1->pos = read_encoder(slave_select_1);
    wheel2->pos = read_encoder(slave_select_2);
    wheel3->pos = read_encoder(slave_select_3);

    wheel1->sequence = encoder_sequence++;
    wheel2->sequence = encoder_sequence;
    wheel3->sequence = encoder_sequence;

    //sanity check for encoder readings
    int difference = abs(wheel1->pos - wheel1->prev_pos);
    if (difference > 250){
        dbgErrorRoutine(ERROR_SPI);
    }
    difference = abs(wheel2->pos - wheel2->prev_pos);
    if (difference > 250){
        dbgErrorRoutine(ERROR_SPI);
    }
    difference = abs(wheel3->pos - wheel3->prev_pos);
    if (difference > 250){
        dbgErrorRoutine(ERROR_SPI);
    }

    msg.data_len = encode_SPI(msg.payload, *wheel1);
    if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
        broker_push(&msg);
    msg.data_len = encode_SPI(msg.payload, *wheel2);
    if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
        broker_push(&msg);
    msg.data_len = encode_SPI(msg.payload, *wheel3);
    if (msg.data_len > 0 && msg.data_len <= PAYLOAD_SIZE)
        broker_push(&msg);
}

int init_encoder(unsigned index) {

    // encoder 1
    GPIO_write(index, 0);
    int status = set_mode_SPI();
    GPIO_write(index, 1);

    GPIO_write(index, 0);
    status = clear_SPI();
    GPIO_write(index, 1);

    return status;
}

uint32_t read_encoder(unsigned index) {
    dbgOutputLoc(READ_ENCODER);
    GPIO_write(index, 0);

    uint8_t byte0;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;

    int status = read_SPI(&byte0, &byte1, &byte2, &byte3);

    uint32_t result = (byte3 << 8) + byte2;
    result = (result << 8) + byte1;
    result = (result << 8) + byte0;

    GPIO_write(index, 1);

    return result;
}

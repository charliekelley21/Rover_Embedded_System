/*
 * encoder_queue.c
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#include "encoder_queue.h"

QueueHandle_t create_encoder_queue()
{
    encoder_qh = xQueueCreate(10, sizeof(time_msg));
    return encoder_qh;
}

int encoder_push_msg(int msg_id) {
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    time_msg msg = {msg_id};
    xQueueSendFromISR(encoder_qh, (void *)&msg, &xHigherPriorityTaskWoken);
    return success;
}

int encoder_pop(int *encoder_case) {
    int success = 1;
    time_msg msg;
    xQueueReceive(encoder_qh, &msg, portMAX_DELAY);
    *encoder_case = msg.id;
    return success;
}

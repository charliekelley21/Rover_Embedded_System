/*
 * state_queue.c
 *
 *  Created on: Oct 28, 2020
 *      Author: chuck
 */

#include "state_queue.h"

QueueHandle_t create_state_queue()
{
    state_qh = xQueueCreate(10, sizeof(state_t));
    return state_qh;
}

int state_push(struct state_t state) {
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(state_qh, (void *)&state, &xHigherPriorityTaskWoken);

    return success;
}


int state_pop(struct state_t* state){
    int success = 1;
    xQueueReceive(state_qh, state, portMAX_DELAY);
    return success;
}

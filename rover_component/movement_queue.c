/*
 * movement_queue.c
 *
 *  Created on: Oct 20, 2020
 *      Author: chuck
 */

#include "movement_queue.h"

QueueHandle_t create_movement_queue()
{
    movement_qh = xQueueCreate(10, sizeof(move_t));
    return movement_qh;
}

int movement_push(struct move_t move) {
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(movement_qh, (void *)&move, &xHigherPriorityTaskWoken);

    return success;
}


int movement_pop(struct move_t* move){
    int success = 1;
    xQueueReceive(movement_qh, move, portMAX_DELAY);
    return success;
}

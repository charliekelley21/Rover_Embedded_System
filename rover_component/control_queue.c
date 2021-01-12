/*
 * control_queue.c
 *
 *  Created on: Oct 7, 2020
 *      Author: chuck
 */

#include "control_queue.h"

QueueHandle_t create_control_queue()
{
    control_qh = xQueueCreate(10, sizeof(wheel_t));
    return control_qh;
}

int control_push_wheel(struct wheel_t control) {
    int success = 1;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    xQueueSendFromISR(control_qh, (void *)&control, &xHigherPriorityTaskWoken);

    return success;
}


int control_pop(struct wheel_t* control_wheel){
    int success = 1;
    xQueueReceive(control_qh, control_wheel, portMAX_DELAY);
    return success;
}

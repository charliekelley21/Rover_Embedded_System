/*
 * control_queue.h
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#ifndef CONTROL_QUEUE_H_
#define CONTROL_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include "json_parser.h"

QueueHandle_t control_qh;

QueueHandle_t create_control_queue();
int control_push_wheel(struct wheel_t control);
int control_pop(struct wheel_t* control_wheel);

#endif /* CONTROL_QUEUE_H_ */

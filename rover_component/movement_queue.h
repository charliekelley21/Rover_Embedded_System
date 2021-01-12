/*
 * movement_queue.h
 *
 *  Created on: Oct 20, 2020
 *      Author: chuck
 */

#ifndef MOVEMENT_QUEUE_H_
#define MOVEMENT_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include "json_parser.h"

QueueHandle_t movement_qh;

QueueHandle_t create_movement_queue();
int movement_push(struct move_t move);
int movement_pop(struct move_t* move);

#endif /* MOVEMENT_QUEUE_H_ */

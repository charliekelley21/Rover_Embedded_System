/*
 * state_queue.h
 *
 *  Created on: Oct 28, 2020
 *      Author: chuck
 */

#ifndef STATE_QUEUE_H_
#define STATE_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>
#include "json_parser.h"

QueueHandle_t state_qh;

QueueHandle_t create_state_queue();
int state_push(struct state_t state);
int state_pop(struct state_t* state);

#endif /* STATE_QUEUE_H_ */

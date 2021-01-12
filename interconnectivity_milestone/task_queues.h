/*
 * task_queues.h
 *
 *  Created on: Sep 25, 2020
 *      Author: chuck
 */

#ifndef TASK_QUEUES_H_
#define TASK_QUEUES_H_

#include "interconnectivity_milestone.h"

QueueHandle_t task1_v1_qh;
QueueHandle_t task1_v2_qh;
QueueHandle_t task2_qh;

void create_task_queues();

// functions to send messages to the task 1 v.1 queue
int task1_v1_push(int type, int value);
struct ReceivedData task1_v1_pop();

// functions to send messages to the task 1 v.2 queue
int task1_v2_push(struct IR_avg *msg_in);
struct IR_avg task1_v2_pop();
int task1_v2_size();

// queue functiosn for v.2
int task2_push(struct chain_message *pack);
struct chain_message task2_pop();

#endif /* TASK_QUEUES_H_ */

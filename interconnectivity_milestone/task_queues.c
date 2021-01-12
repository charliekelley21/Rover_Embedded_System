/*
 * task_queues.c
 *
 *  Created on: Sep 25, 2020
 *      Author: chuck
 */

#include "task_queues.h"

//------------------------------------------------------create task queues
void create_task_queues() {
    // queue for task1 - just to hold data from IR sensor
    task1_v1_qh = xQueueCreate(5, sizeof(ReceivedData));

    // queue for task1 - just to hold data from IR sensor
    task1_v2_qh = xQueueCreate(15, sizeof(struct IR_avg));

    // queue for task2 - holds chain_messages
    task2_qh = xQueueCreate(5, sizeof(struct chain_message));


}

//------------------------------------------------------1.1 queue
struct ReceivedData task1_v1_pop()
{
    // this is simple - throw data right into a struct
    struct ReceivedData buff;

    if (xQueueReceive(task1_v1_qh, &buff, portMAX_DELAY) == pdFALSE)
    {
        dbgOutputLoc(RECEIVE_ERROR);
//        dbgErrorRoutine(RECEIVE_ERROR);
    }

    return buff;
}

int task1_v1_push(int type, int value)
{
    // leading 0s means sensor msg
    struct ReceivedData buff;
    buff.type = type;
    buff.value = value;

    dbgOutputLoc(BEFORE_SENDING);
    if (xQueueSendFromISR(task1_v1_qh, &buff, NULL) == errQUEUE_FULL)
    {
        dbgOutputLoc(ERROR_QUEUE_FULL);
        return -1; // indicate error
    }
    dbgOutputLoc(AFTER_SENDING);

    // indicates success
    return 0;
}

//------------------------------------------------------1.2 queue
struct IR_avg task1_v2_pop()
{
    // this is simple - throw data right into a struct
    struct IR_avg buff;
    buff.sequence = -1;

    if (xQueueReceive(task1_v2_qh, &buff, 0) == pdFALSE)
    {
        dbgOutputLoc(RECEIVE_ERROR);
//        dbgErrorRoutine(RECEIVE_ERROR);
    }

    return buff;
}

int task1_v2_push(struct IR_avg *msg_in)
{
    dbgOutputLoc(BEFORE_SENDING);
    if (xQueueSendFromISR(task1_v2_qh, msg_in, NULL) == errQUEUE_FULL)
    {
        dbgOutputLoc(ERROR_QUEUE_FULL);
        return -1; // indicate error
    }
    dbgOutputLoc(AFTER_SENDING);

    // indicates success
    return 0;
}

int task1_v2_size() {
    return uxQueueMessagesWaiting(task1_v2_qh);
}

//------------------------------------------------------2 queue
int task2_push(struct chain_message *pack) {

    dbgOutputLoc(BEFORE_SENDING);
    if (xQueueSendFromISR(task2_qh, pack, NULL) == errQUEUE_FULL)
    {
        dbgOutputLoc(ERROR_QUEUE_FULL);
        return -1; // indicate error
    }
    dbgOutputLoc(AFTER_SENDING);

    // indicates success
    return 0;
}

struct chain_message task2_pop()
{
    // this is simple - throw data right into a struct
    struct chain_message buff;

    if (xQueueReceive(task2_qh, &buff, portMAX_DELAY) == pdFALSE)
    {
        dbgOutputLoc(RECEIVE_ERROR);
//        dbgErrorRoutine(RECEIVE_ERROR);
    }

    return buff;
}

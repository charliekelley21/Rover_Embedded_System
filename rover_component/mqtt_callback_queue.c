#include "mqtt_callback_queue.h"

void create_queues() {
    // instantiate the queue - allow it to hold 3 of our struct
    qh = xQueueCreate(5, sizeof(struct msgQueue));

    // queue for forwarding data through MQTT callback
    mqtt_qh = xQueueCreate(5, sizeof(struct msgQueue));
}

int broker_push(struct msgQueue *msg) {
    if (xQueueSendFromISR(qh, msg, NULL) == errQUEUE_FULL)
    {
        dbgOutputLoc(ERROR_QUEUE_FULL);
//        dbgErrorRoutine(ERROR_QUEUE_FULL);
    }

    // indicates success
    return 0;
}

struct msgQueue broker_pop() {
    // this is simple - throw data right into a struct
    struct msgQueue buff;

    if (xQueueReceive(qh, &buff, portMAX_DELAY) == pdFALSE)
    {
        dbgOutputLoc(RECEIVE_ERROR);
        dbgErrorRoutine(ERROR_QUEUE_RECEIVE);
    }

    return buff;
}

int sendMsgToQueue(QueueHandle_t qh_, struct msgQueue *msg)
{
    if (xQueueSendFromISR(qh_, msg, NULL) == errQUEUE_FULL)
    {
        dbgOutputLoc(ERROR_QUEUE_FULL);
//        dbgErrorRoutine(ERROR_QUEUE_FULL);
    }

    // indicates success
    return 0;
}

struct msgQueue receiveData(QueueHandle_t qh_)
{
    // this is simple - throw data right into a struct
    struct msgQueue buff;

    if (xQueueReceive(qh_, &buff, portMAX_DELAY) == pdFALSE)
    {
        dbgOutputLoc(RECEIVE_ERROR);
        dbgErrorRoutine(ERROR_QUEUE_RECEIVE);
    }

    return buff;
}

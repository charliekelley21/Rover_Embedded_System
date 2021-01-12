#ifndef MQTT_CALLBACK_QUEUE_H_
#define MQTT_CALLBACK_QUEUE_H_

#include <rover_component.h>

#define PAYLOAD_SIZE        512
#define TOPIC_SIZE          20

// all we need is the event and the topic, I guess
struct msgQueue
{
    int     event;                      // hold instructions about event to process
    char    topic[TOPIC_SIZE];          // hold topic name
    char    payload[PAYLOAD_SIZE];      // hold JSON string of data
    int     data_len;                   // hold length of data
};

// struct that gets passed around the state machine
typedef struct ReceivedData {
    int type; // 0 = time, 1 = sensor data
    int value;
} ReceivedData;

// queue handle to hold topics to which we will publish
QueueHandle_t qh;

// internal MQTT callback queue
QueueHandle_t mqtt_qh;

// this is for sending data to the queue, to be published
int sendMsgToQueue(QueueHandle_t qh_, struct msgQueue *msg);

struct msgQueue receiveData(QueueHandle_t qh_);

void create_queues();

int broker_push(struct msgQueue *msg);
struct msgQueue broker_pop();

#endif /* MQTT_CALLBACK_QUEUE_H_ */

#ifndef INTERCONNECTIVITY_MILESTONE_H_
#define INTERCONNECTIVITY_MILESTONE_H_

#include <mqtt_if.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "task.h"
#include <queue.h>
#include "jsmn.h"

#include <ti/drivers/net/wifi/simplelink.h>
#include <ti/drivers/net/wifi/slnetifwifi.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/Timer.h>
#include <ti/net/mqtt/mqttclient.h>
#include <event_groups.h>
#include "event_group.h"

#include "network_if.h"
#include "uart_term.h"
#include "mqtt_if.h"
#include "debug_if.h"

// my files
#include "debug.h"
#include "ti_drivers_config.h"
#include "json_parser.h"
#include "task_queues.h"
#include "A2D.h"
#include "task1.h"
#include "task2.h"
#include "mqtt_callback_queue.h"

/* Stack size in bytes */
#define THREADSTACKSIZE   2048

// which part of the chain you are
#define SPENCER         1 // IR publisher
#define MICHAEL         2 // IR receiver
#define CHARLIE         3 // IR publisher
#define CHRISTOS        4 // IR receiver

// Everyone specify their chain num here
#define CHAIN_NUM       SPENCER

// 0 is publisher, 1 is receiver
//#define TASK1_MODE      0
#if CHAIN_NUM == 2 || CHAIN_NUM == 4
    #define TASK1_MODE  1
#else
    #define TASK1_MODE  0
#endif

UART_Handle uartHandle;
MQTTClient_Handle mqttClientHandle;

// hold analytics data
struct debug_msg dbg_msg;

#define TEST_MSG            98
#define BROKER_MSG          99
#define LED1_MSG            100
#define COUNT_CB_EVENT      101
#define APP_MQTT_PUBLISH    102
#define IR_READ_EVENT       103

// publish events
#define TASK1_PUB_SENSOR    50
#define TASK1_PUB_AVERAGE   51
#define PUB_ANALYTICS       52
#define TASK2_PUB_MSG       53

#endif /* INTERCONNECTIVITY_MILESTONE_H_ */

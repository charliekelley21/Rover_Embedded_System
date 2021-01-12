#ifndef ROVER_COMPONENT_H_
#define ROVER_COMPONENT_H_

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

#include "network_if.h"
#include "uart_term.h"
#include "uart_rover.h"
#include "encoder_SPI.h"
#include "mqtt_if.h"
#include "debug_if.h"

// my files
#include "debug.h"
#include "ti_drivers_config.h"
#include "json_parser.h"
#include "mqtt_callback_queue.h"

#include "event_group.h"
#include "rover_timers.h"

#include "encoder_queue.h"
#include "encoder_thread.h"

#include "control_queue.h"
#include "control_thread.h"

#include "movement_thread.h"
#include "movement_queue.h"

#include "state_queue.h"
#include "state_thread.h"

/* Stack size in bytes */
#define THREADSTACKSIZE   2048

UART_Handle uartHandle;
MQTTClient_Handle mqttClientHandle;

#define TEST_MSG            98
#define BROKER_MSG          99
#define APP_MQTT_PUBLISH    102

// publish events
#define ENCODER_MSG         49
#define CONTROL_MSG         50
#define STATE_MSG           51
#define ODOM_MSG            52

#endif /* ROVER_COMPONENT_H_ */

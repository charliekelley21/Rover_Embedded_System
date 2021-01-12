#ifndef MAPPING_COMPONENT_H_
#define MAPPING_COMPONENT_H_

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
#include <ti/drivers/PWM.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Timer.h>
#include <ti/net/mqtt/mqttclient.h>
#include <event_groups.h>

#include "network_if.h"
#include "uart_term.h"
#include "mqtt_if.h"
#include "debug_if.h"

// my files
#include "debug.h"
#include "ti_drivers_config.h"
#include "json_parser.h"
#include "task_queues.h"
#include "event_group.h"
#include "mqtt_callback_queue.h"

// tasks
#include "taskUART_read.h"
#include "taskLIDAR.h"

/* Stack size in bytes */
#define THREADSTACKSIZE   2048

// hold analytics/debug info
struct debug_msg dbg_msg;

// publish events
#define ANALYTICS_PUB           100
#define LIDAR_POINT_PUB         101

#endif /* MAPPING_COMPONENT_H_ */

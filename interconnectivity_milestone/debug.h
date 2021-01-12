/*
 * debug.h
 *
 *  Created on: Sep 7, 2020
 *      Author: Spencer
 */

#ifndef DEBUG_H_
#define DEBUG_H_

// TI-Driver includes
#include <ti/drivers/UART.h>
#include <ti/drivers/GPIO.h>
#include "ti_drivers_config.h"
#include <debug_if.h>

// define all states to output at different times
#define BEFORE_SENDING              1
#define AFTER_SENDING               2
#define BEFORE_RECEIVING            3
#define AFTER_RECEIVING             4

#define ISR_90MS_ENTER              5
#define ISR_500MS_ENTER             6
#define ISR_90MS_EXIT               7
#define ISR_500MS_EXIT              8

// ALL ERRORS ARE IN 100s
#define HARDWARE_TIMER_ERROR        100
#define UART_INIT_ERROR             101
#define RECEIVE_ERROR               102
#define ERROR_QUEUE_FULL            103
#define ERROR_TASK_CREATE           104
#define ERROR_QUEUE_CREATE          105
#define ERROR_RTOS_STACK_OVERFLOW   106
#define ERROR_INIT_ADC              107
#define ERROR_QUEUE_RECEIVE         108
#define ERROR_WIFI_INIT             109
#define ERROR_TIMER_INIT            110
#define ERROR_AP_CONNECTION         111
#define ERROR_MQTT_INIT             112
#define ERROR_MQTT_SUB              113
#define ERROR_MQTT_CONNECTION       114
#define TIMER1_OPEN_ERROR           115
#define TIMER1_START_ERROR          116
#define ERROR_MSG_RECV_TOO_BIG      117
#define ERROR_JSMN_PARSE            118
#define ERROR_EVENT_GROUP           119
#define ERROR_RTOS_MALLOC           120


#define GET_IR_SENSOR_READING       200


#define ENTER_MAIN_TASK             77
#define BEFORE_WHILE_1              72

// 7 is MSB
#define GPIO7_MASK                  0b1000000
#define GPIO6_MASK                  0b100000
#define GPIO5_MASK                  0b10000
#define GPIO4_MASK                  0b1000
#define GPIO3_MASK                  0b100
#define GPIO2_MASK                  0b10
#define GPIO1_MASK                  0b1

// just spit out a character
void dbgUARTVal(UART_Handle uartHandle, char *outVal);

// output a value through GPIOs that says where we are in code
void dbgOutputLoc(unsigned int outLoc);

void dbgErrorRoutine(int failLoc);

#endif /* DEBUG_H_ */

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

#define CONTROL_TASK                10
#define CONTROL_PID                 11

#define ENCODER_TASK                14
#define READ_ENCODER                15

// ALL ERRORS ARE IN 100s
#define HARDWARE_TIMER_ERROR        20
#define UART_INIT_ERROR             21
#define RECEIVE_ERROR               22
#define ERROR_QUEUE_FULL            23
#define ERROR_TASK_CREATE           24
#define ERROR_QUEUE_CREATE          25
#define ERROR_RTOS_STACK_OVERFLOW   26
#define ERROR_QUEUE_RECEIVE         28
#define ERROR_WIFI_INIT             29
#define ERROR_TIMER_INIT            30
#define ERROR_AP_CONNECTION         31
#define ERROR_MQTT_INIT             32
#define ERROR_MQTT_SUB              33
#define ERROR_MQTT_CONNECTION       34
#define ERROR_MSG_RECV_TOO_BIG      37
#define ERROR_JSMN_PARSE            38
#define ERROR_EVENT_GROUP           39
#define ERROR_RTOS_MALLOC           40
#define ERROR_SPI                   41
#define ERROR_CONTROL               42
#define ERROR_UART                  43
#define ERROR_STATE_MACHINE         44
#define ERROR_PID                   45

#define GET_SPI_READING             60

// 5 is MSB (0, 64)
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

/*
 * uart_rover.h
 *
 *  Created on: Oct 7, 2020
 *      Author: chuck
 */

#ifndef UART_ROVER_H_
#define UART_ROVER_H_

#include "rover_component.h"

#include <unistd.h>
#include <stddef.h>
#include "ti_drivers_config.h"
#include <ti/drivers/UART.h>
#include "debug.h"

#define CHECKSUM        0b01111111

#define FORWARD         1
#define BACKWARD        0

#define MOTOR_1         128
#define MOTOR_2         129
#define MOTOR_3         130


UART_Handle rover_uart;

void motor_driver_init();
void wheel_write(uint8_t address, uint8_t direction, int ticks);

#endif /* UART_ROVER_H_ */

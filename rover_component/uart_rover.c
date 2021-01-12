/*
 * uart_rover.c
 *
 *  Created on: Oct 7, 2020
 *      Author: chuck
 */

#include "uart_rover.h"

void motor_driver_init() {
    UART_Params uartParams;

    UART_Params_init(&uartParams);
    uartParams.writeMode = UART_MODE_BLOCKING;
    uartParams.readMode = UART_MODE_BLOCKING;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 9600;
    uartParams.readEcho = UART_ECHO_OFF;

    rover_uart = UART_open(motor_driver_uart, &uartParams);

    if (rover_uart == NULL) {
        dbgErrorRoutine(ERROR_UART);
    }
    sleep(2);
    uint8_t baudChar = 170;
    UART_write(rover_uart, &baudChar, 1);
}

void wheel_write(uint8_t address, uint8_t direction, int speed) {
    // {0, 127}
    uint8_t checksum = (address + direction + speed) & CHECKSUM;
    uint8_t data[] = {address, direction, speed, checksum};
    UART_write(rover_uart, data, sizeof(data));
}

/*
 * encoder_thread.h
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#ifndef ENCODER_THREAD_H_
#define ENCODER_THREAD_H_

#include "encoder_SPI.h"
#include <ti/drivers/UART.h> //uart test
#include "uart_rover.h"
#include "ti_drivers_config.h"
#include <ti/drivers/GPIO.h>
#include <unistd.h>
#include "encoder_queue.h"
#include "control_queue.h"
#include "rover_timers.h"
#include "json_parser.h"

void create_encoder_task();
void *encoder_thread(void *arg0);
int init_encoder(unsigned index);
uint32_t read_encoder(unsigned index);
void rover_position(struct wheel_t* wheel1, struct wheel_t* wheel2, struct wheel_t* wheel3);

#endif /* ENCODER_THREAD_H_ */

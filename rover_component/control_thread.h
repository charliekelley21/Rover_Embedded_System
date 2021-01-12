/*
 * control_thread.h
 *
 *  Created on: Oct 7, 2020
 *      Author: chuck
 */

#ifndef CONTROL_THREAD_H_
#define CONTROL_THREAD_H_

#include <unistd.h>
#include <math.h>

#include <ti/drivers/UART.h>
#include "uart_rover.h"
#include "encoder_queue.h"
#include "control_queue.h"
#include "json_parser.h"

void create_control_task();
void *control_thread(void *arg0);
void pi_algorithm(struct controller_t* ref, struct wheel_t wheel, float Kp, float Ki);

#endif /* CONTROL_THREAD_H_ */

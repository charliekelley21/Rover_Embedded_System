/*
 * encoder_queue.h
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#ifndef ENCODER_QUEUE_H_
#define ENCODER_QUEUE_H_

#include <FreeRTOS.h>
#include <queue.h>

typedef struct {
    int id;
} time_msg;

#define TIME_ID         0
#define START_ID        1
#define SPI_ID          2

QueueHandle_t encoder_qh;

QueueHandle_t create_encoder_queue();
int encoder_push_msg(int msg_id);
int encoder_pop(int *encoder_case);


#endif /* ENCODER_QUEUE_H_ */

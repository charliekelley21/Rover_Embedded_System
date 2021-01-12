/*
 * encoder_SPI.h
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#ifndef ENCODER_SPI_H_
#define ENCODER_SPI_H_

#include <stddef.h>
#include <string.h>
#include <ti/drivers/SPI.h>
#include <unistd.h>
#include "debug.h"
#include "encoder_queue.h"

/* Driver configuration */
#include "ti_drivers_config.h"

SPI_Handle tri_encoder;

void init_SPI();
void CB_SPI(SPI_Handle handle, SPI_Transaction *transaction);
int read_SPI(uint8_t *byte0, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3);
int set_mode_SPI();
int clear_SPI();
int execute_instruction(uint8_t *rx_buffer, uint8_t *tx_buffer, uint8_t tran_count);


#endif /* ENCODER_SPI_H_ */

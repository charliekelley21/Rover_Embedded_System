/*
 * encoder_SPI.c
 *
 *  Created on: Oct 5, 2020
 *      Author: chuck
 */

#include "encoder_SPI.h"

void init_SPI() {
    SPI_Params spiParams;
    SPI_Params_init(&spiParams);
    spiParams.bitRate = 50000;
    tri_encoder = SPI_open(CONFIG_SPI_0, &spiParams);
    if (tri_encoder == NULL) {
        dbgErrorRoutine(ERROR_SPI);
    }
}

void CB_SPI(SPI_Handle handle, SPI_Transaction *transaction) {
    encoder_push_msg(SPI_ID);
}

int read_SPI(uint8_t *byte0, uint8_t *byte1, uint8_t *byte2, uint8_t *byte3) {
    dbgOutputLoc(GET_SPI_READING);

    // {0x60} -> transers CNTR to OTR register, transmits OTR serially
    uint8_t TX[] = {0x60, 0, 0, 0, 0}; // 4 junks
    uint8_t RX[] = {0, 0, 0, 0, 0};  // junk
    int ret = execute_instruction(RX, TX, sizeof(TX));

    *byte0 = RX[4];
    *byte1 = RX[3];
    *byte2 = RX[2];
    *byte3 = RX[1];

    return ret;
}

int set_mode_SPI() {
    // write to MDR0:
    // {0x03} -> clock division = 0, asynch, disable index,
    //           free-running count mode, x4 quadrature count mode
    uint8_t TX[] = {0x88, 0x03};
    uint8_t RX[] = {0, 0};
    return execute_instruction(RX, TX, sizeof(TX));
}

int clear_SPI() {
    // {0x20} -> clear CNTR
    uint8_t TX[] = {0x20};
    uint8_t RX[] = {0}; // junk
    return execute_instruction(RX, TX, sizeof(TX));
}

int execute_instruction(uint8_t *rx_buffer, uint8_t *tx_buffer, uint8_t tran_count) {

    SPI_Transaction transaction;
    transaction.txBuf = (void *) tx_buffer;
    transaction.rxBuf = (void *) rx_buffer;
    transaction.count = tran_count;
    return SPI_transfer(tri_encoder, &transaction);
}

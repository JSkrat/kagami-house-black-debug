#include "nRF model.h"
#include "sstring.h"
#include "../usb2nrf/ui.h"
#include "../usb2nrf/nrf24l01.h"
#include "../usb2nrf/nrf24l01-mnemonics.h"
#include <string.h>
#include <stdlib.h>

nRF24L01 *rfTransiever;
fNRFCallback cNRF_DataTransmitted = NULL, cNRF_DataReceived = NULL, cNRF_TransmissionFailed = NULL;

void nRF_init() {

}


int nRF24L01_transmit_success(nRF24L01 *rf) {
    (void) rf;
    return 0;
}

void nRF24L01_transmit(nRF24L01 *rf, void *address, nRF24L01Message *msg) {
    (void) rf;
    (void) address;
    (void) msg;
}

uint8_t nRF24L01_read_register(nRF24L01 *rf, uint8_t regAddress, void *data,
                               size_t length) {
    (void) rf;
    (void) regAddress;
    (void) data;
    (void) length;
    return 0;
}

bool nRF24L01_data_received(nRF24L01 *rf) {
    (void) rf;
    return false;
}

bool messageHaveBeenReceived;
nRF24L01Message receivedMessage;
bool nRF24L01_read_received_data(nRF24L01 *rf, nRF24L01Message *message) {
    (void) rf;
    if (! messageHaveBeenReceived) {
        messageHaveBeenReceived = true;
        message = &receivedMessage;
        return true;
    } else {
        return false;
    }
}

int listenCalls = 0;
static uint8_t llaData[256];
sString lastListenAddress = {255, &llaData[0]};
void nRF24L01_listen(nRF24L01 *rf, int pipe, uint8_t *address) {
    (void) rf;
    (void) pipe;
    listenCalls++;
    lastListenAddress.length = MAC_SIZE;
    memcpy(lastListenAddress.data, address, MAC_SIZE);
}

uint8_t nRF24L01_write_register(nRF24L01 *rf, uint8_t reg_address, void *data,
                                size_t length) {
    (void) rf;
    (void) reg_address;
    (void) data;
    (void) length;
    return 0;
}

void nRF24L01_flush_transmit_message(nRF24L01 *rf) {
    (void) rf;
}

void _delay_us(int delay)
{
    (void) delay;
}

void ui_subsystem_str(ui_subsystems subsystem, const string *message, bool in_flash) {
    (void) subsystem;
    (void) message;
    (void) in_flash;
}

void nRF_init_for_tests()
{
//    lastListenAddress.data = malloc(255);
//    lastListenAddress.length = 255;
    listenCalls = 0;
}

void nRF_listen(const uint8_t *address) {
    nRF24L01_listen(rfTransiever, 0, (uint8_t *)address);
}

void nRF_transmit(uint8_t *address, uint8_t length, uint8_t *data) {
    nRF24L01Message msg;
    msg.length = length;
    memcpy(&(msg.data), data, length);
    nRF24L01_transmit(rfTransiever, address, &msg);
}

void nRF_go_idle()
{

}

bool nRF_validate_rf_channel(uint8_t channel)
{

}

bool nRF_setRFChannel(uint8_t channel)
{

}


#ifndef NRF_MODEL_H_
#define NRF_MODEL_H_


#include <stdint.h>
#include "sstring.h"
#include "../usb2nrf/defines.h"
#include "../usb2nrf/nrf24l01.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01-mnemonics.h"
#include "../usb2nrf/avr-nrf24l01-master/src/nrf24l01.h"

extern nRF24L01 *rfTransiever;

typedef void (*fNRFCallback)(sString *address, sString *payload);
extern fNRFCallback cNRF_DataTransmitted, cNRF_DataReceived, cNRF_TransmissionFailed;

void nRF_init_for_tests();

void nRF_init();

extern int listenCalls;
extern sString lastListenAddress;
void nRF_listen(const uint8_t *address);

extern nRF24L01Message receivedMessage;
extern bool messageHaveBeenReceived;

void nRF_transmit(uint8_t *address, uint8_t length, uint8_t *data);

void nRF_go_idle();
bool nRF_validate_rf_channel(uint8_t channel);
bool nRF_setRFChannel(uint8_t channel);

#endif /* NRF_MODEL_H_ */

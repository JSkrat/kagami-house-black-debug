#ifndef RFPARSER_H
#define RFPARSER_H

#include <stdint.h>
#include "../usb2nrf/defines.h"

typedef uint8_t t_address[MAC_SIZE];

void setListenAddress(t_address* data);

#endif // RFPARSER_H

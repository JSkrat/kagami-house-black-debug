#ifndef PGMSPACE_H
#define PGMSPACE_H

#include <stdint.h>
#include "../usb2nrf/nrf24l01.h"
#include "../usb2nrf/nrf24l01-mnemonics.h"
#include "sstring.h"

#define PROGMEM __attribute__((__section__("progmem")))
//#define const

#ifdef __cplusplus
extern "C" {
#endif
    void initialize();
    void reinitialize_memory();

    void *pgm_read_ptr_near(const void *ptr);
    void *pgm_read_ptr(const void *ptr);

    uint8_t pgm_read_byte(const uint8_t *ptr);
#ifdef __cplusplus
}
#endif

void _delay_us(int delay);


#endif // PGMSPACE_H

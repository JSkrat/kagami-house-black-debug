#include "pgmspace.h"
#include <stdint.h>
#include <string.h>
//#include "sstring.h"


extern uint8_t *__start_progmem;
extern uint8_t *__stop_progmem;
// i have no idea why its size is size of pointer, not size of data
// also have no idea why we need to get a pointer to that, not directly that
#define PROGMEM_SIZE (&__stop_progmem - &__start_progmem)*sizeof(__start_progmem)
// without this magical variable memcpy back to segment causes sigsegv for some reason
// no matter where in this file it is 6_9
static uint8_t qwerty PROGMEM = 0;

// an original data before any tests
static uint8_t *memory_copy;


void initialize() {
    (void) qwerty;
    memory_copy = malloc(PROGMEM_SIZE);
    memcpy(memory_copy, &__start_progmem, PROGMEM_SIZE);
}

void reinitialize_memory() {
    /// here we're inverting the whole section so direct access will return not what is there, failing the test
    /// only allowed access is through pgm_* functions
    // fill up the data
    memcpy(&__start_progmem, memory_copy, PROGMEM_SIZE);
    // and now lets invert it
    uint8_t *iByte = (uint8_t*)(&__start_progmem);
    while (iByte < (uint8_t*)(&__stop_progmem)) {
        *iByte = ~(*iByte);
        iByte++;
    }
}

/******************************************************
 * pgm* functions emulation
 ******************************************************/
void *pgm_read_ptr_near(void const *ptr) {
//    return *((void **)ptr);
    return pgm_read_ptr(ptr);
}

void *pgm_read_ptr(void const *ptr) {
    /// todo might fail on 128-bit machines
    return (void *)(~(*(const uint64_t*)(ptr)));
}

uint8_t pgm_read_byte(const uint8_t *ptr) {
    return ~(*ptr);
}

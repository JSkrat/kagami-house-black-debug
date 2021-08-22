/*
 * RF_custom_functions.c
 *
 * Created: 23.05.2020 16:53:54
 *  Author: Mintytail
 */ 

#if BT_SLAVE == BUILD_TYPE
#include "RF custom functions.h"
#include "../KagamiCore/RF functions.h"
#include "../KagamiCore/RF protocol.h"
#include <avr/pgmspace.h>
#include "../KagamiCore/defines.h"
#include <avr/interrupt.h>
#include "../project settings.h"
#include "ui.h"

typedef struct {
	volatile uint8_t *port;
	uint8_t pin;
} gpio_pin;

typedef struct {
	gpio_pin pin;
	uint8_t unit;
	uint8_t function;
} tMapFunctionToPinItem;

#define MAP_SIZE 4
const tMapFunctionToPinItem map[MAP_SIZE];

ISR(TIMER0_OVF_vect) {
	portServo &= ~_BV(poServo);
	ui_tick();
}

ISR(TIMER0_COMPA_vect) {
	portServo |= _BV(poServo);
}

void RFCustomFunctionsInit() {
	DDRD |= (1 << poLED_D1) | (1 << poLED_D2) | (1 << poServo);
	DDRD &= ~((1 << piSW1) | (1 << piSW2));
	// timer for the servo
	TCCR0A = 0;
	TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00); // clkio/1024
	OCR0A = 0;
	TIMSK0 = _BV(TOIE0) | _BV(OCIE0A);
	functions_init(Units, unitsCount);
}

bool findPin(const uint8_t unit, const uint8_t function, gpio_pin *output) {
	for (int i = 0; i < MAP_SIZE; i++) {
		uint8_t u = pgm_read_byte(&(map[i].unit));
		if (unit != u) continue;
		uint8_t f = pgm_read_byte(&(map[i].function));
		if (function != f) continue;
		// found it
		output->port = pgm_read_ptr(&(map[i].pin.port));
		output->pin = pgm_read_byte(&(map[i].pin.pin));
		return true;
	}
	return false;
}

uint8_t readPin(const uint8_t unit, const uint8_t function, const scString *request, sString *response) {
	(void) request;
	gpio_pin pin;
	if (! findPin(unit, function, &pin)) return 0x80;
	response->length = 1;
	response->data[0] = 0;
	if (*pin.port & _BV(pin.pin)) response->data[0] = 1;
	return ercOk;
}

uint8_t writePin(const uint8_t unit, const uint8_t function, const scString *request, sString *response) {
	(void) response;
	if (1 != request->length) return ercBadRequestData;
	gpio_pin pin;
	if (! findPin(unit, function, &pin)) return 0x80;
	if (0 == request->data[0]) *pin.port &= ~_BV(pin.pin);
	else if (1 == request->data[0]) *pin.port |= _BV(pin.pin);
	else return ercBadRequestData;
	return ercOk;
}

uint8_t readRegister(const uint8_t unit, const uint8_t function, const scString *request, sString *response) {
	response->length = 1;
	response->data[0] = OCR0A;
	return ercOk;
}

uint8_t writeRegister(const uint8_t unit, const uint8_t function, const scString *request, sString *response) {
	(void) response;
	if (1 != request->length) return ercBadRequestData;
	OCR0A = request->data[0];
	return ercOk;
}

const PROGMEM tMapFunctionToPinItem map[MAP_SIZE] = {
	{ .unit = 1, .function = 0x10, .pin = {.port = &PIND, .pin = piSW1} },
	{ .unit = 1, .function = 0x12, .pin = {.port = &PIND, .pin = piSW2} },
	{ .unit = 1, .function = 0x15, .pin = {.port = &PORTD, .pin = poLED_D1} },
	{ .unit = 1, .function = 0x17, .pin = {.port = &PORTD, .pin = poLED_D2} },
};

const PROGMEM tRFCodeFunctionItem U1Functions[fU1Count] = {
	{
		.functionCode = 0x10, .type.fields.input = edtNone, .type.fields.output = edtBool,
		.function = &readPin
	},
	{
		.functionCode = 0x12, .type.fields.input = edtNone, .type.fields.output = edtBool,
		.function = &readPin
	},
	{
		.functionCode = 0x15, .type.fields.input = edtBool, .type.fields.output = edtNone,
		.function = &writePin
	},
	{
		.functionCode = 0x17, .type.fields.input = edtBool, .type.fields.output = edtNone,
		.function = &writePin
	},
	{
		.functionCode = 0x18, .type.fields.input = edtNone, .type.fields.output = edtByte,
		.function = &readRegister
	},
	{
		.functionCode = 0x19, .type.fields.input = edtByte, .type.fields.output = edtNone,
		.function = &writeRegister
	},
};

const PROGMEM tUnit Units[unitsCount] = {
	{ .length = fU1Count, .functions = U1Functions, .description = esU1Description },
};
#endif
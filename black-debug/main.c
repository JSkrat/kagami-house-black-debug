/*
 * black-debug.c
 *
 * Created: 21.08.2021 15:47:05
 * Author : Mintytail
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "../KagamiCore/UART parser.h"
#include "../KagamiCore/RF model.h"
#include "ui.h"
#include "RF custom functions.h"

ISR(BADISR_vect, ISR_NAKED) {
	// nothing here
}


int main(void)
{
    /* hardware initialization */
#if BT_MASTER == BUILD_TYPE
	u_init();
#endif
#ifdef USE_LCD
	ui_init();
#endif
	rf_init();
	RFCustomFunctionsInit();
	set_sleep_mode(SLEEP_MODE_IDLE);
	sei();
    while (1) {
		sleep_mode();
    }
}


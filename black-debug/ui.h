/*
 * ui.h
 *
 * Created: 03.11.2019 17:24:58
 *  Author: Mintytail
 */ 


#ifndef UI_H_
#define UI_H_

#include "messages.h"
#include <stdbool.h>

enum ui_buttons {
	ui_b_sw1,
	ui_b_sw2,
	ui_b_enc1,
	ui_b_enc2,
	ui_b_enc_btn,
	ui_number_of_buttons,
};

typedef enum {
	ui_s_uart_packet_fsm,
	ui_s_number
} ui_subsystems;

typedef enum {
	ui_v_flash_str,
	ui_v_mem_str,
	ui_v_int16,
	//ui_v_uint16,
} ui_varaint;

void ui_init();
void ui_tick();
void ui_btn_state_changed(enum ui_buttons source, bool state);
void ui_redraw();

void ui_subsystem_str(ui_subsystems subsystem, const string *message, bool in_flash);
void ui_subsystem_int(ui_subsystems subsystem, int16_t value);

// events
void ui_e_main_cycle();


#endif /* UI_H_ */
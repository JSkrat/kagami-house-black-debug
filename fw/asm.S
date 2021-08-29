#define __ASSEMBLY__
#include <avr/io.h>
#include "KagamiCore/defines.h"

#ifdef USE_LCD

.global LCD_write_byte

LCD_write_byte:
; 3 + 15 + 14*9 = 143
#define data r24
#define cd r22
    in r19, SREG - __SFR_OFFSET                     ; 1
    cli                                             ; 1
    ; chip select
    cbi portLCD - __SFR_OFFSET, poLCD_CS            ; 2

    clc                                             ; 1
    sbrc cd, 0                                      ; 2
    sec                                             ;

    ldi r18, 9                                      ; 1

    cyc_bit_send:
        cbi portLCD - __SFR_OFFSET, poLCD_DATA   ; 2
        brcc bit_send                               ; 2
        sbi portLCD - __SFR_OFFSET, poLCD_DATA
        bit_send:

        sbi portLCD - __SFR_OFFSET, poLCD_CLK    ; 2
        ; nop x2 for maximum clk 16MHz
        nop                                         ; 1
        nop                                         ; 1
        cbi portLCD - __SFR_OFFSET, poLCD_CLK    ; 2

        ; next bit
        rol r24                                     ; 1
    dec r18                                         ; 1
    brne cyc_bit_send                               ; 2

    ; chip unselect
    sbi portLCD - __SFR_OFFSET, poLCD_CS            ; 2

    LCD_PROC_EXIT:
    out SREG - __SFR_OFFSET, r19                    ; 1

    ret                                             ; 4

	#endif

#undef __ASSEMBLY__
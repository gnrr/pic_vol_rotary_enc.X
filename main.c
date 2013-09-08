/*
 * File:   main.c
 * Author: g
 *
 * Created on September 8, 2013, 10:26 AM
 */

#include <xc.h>

// PIC18F14K50
#pragma config FOSC   = HS,  PLLEN  = ON,  FCMEN  = OFF
#pragma config IESO   = OFF, USBDIV = OFF, CPUDIV = NOCLKDIV
#pragma config PWRTEN = OFF, BOREN  = OFF, WDTEN  = OFF
#pragma config HFOFST = OFF, MCLRE  = OFF
#pragma config STVREN = ON,  BBSIZ  = OFF, LVP    = OFF
#pragma config XINST  = OFF
#pragma config CP0    = OFF, CP1    = OFF, CPB    = OFF
#pragma config WRT0   = OFF, WRT1   = OFF, WRTB   = OFF, WRTC   = OFF
#pragma config EBTR0  = OFF, EBTR1  = OFF, EBTRB  = OFF

#define _XTAL_FREQ 12000000         // CLK 12MHz (use from __delay_ms)

volatile unsigned char out = 0;

void init(void)
{
    //OSCCON = 0b01010010;         // internal osc 4MHz

    TRISC  = 0b00000000;            // direction: all out
    ANSEL  = 0b00000000;            // analog/digital: all digital
    ANSELH = 0b00000000;            // analog/digital: all digital
}

void main(void)
{
    init();

    while(1) {
        PORTC = out;                 // output to port c
        out++;

        for(unsigned long i = 0;i < 16; i++)   // 65ms * 16 = 1040ms
            __delay_ms(65);             // < 66 ms at 12MHz
    }

    return;
}
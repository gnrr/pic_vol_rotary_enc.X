/*
 * File:   main.c
 *
 * Created on September 8, 2013, 10:26 AM
 */

#include <xc.h>
#include "mystd.h"

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

#define ROT_NA  0
#define ROT_FF  1
#define ROT_RW  2

#define OUT_MAX 15

u1 rot;

void init(void)
{
    // rotary encoder
    INTCON2bits.RABPU = 0;          // PORTB pull up all
    INTCONbits.RABIF = 0;           // clear int flag
    INTCONbits.RABIE = 1;           // enable int (input-change)
    IOCB = 0xC0;                    // bit7..6 as input-change
    INTCONbits.GIE = 1;             // enable global int

    // LED
    TRISC  = 0b00000000;            // direction: all out
    ANSEL  = 0b00000000;            // analog/digital: all digital
    ANSELH = 0b00000000;            // analog/digital: all digital

    rot = ROT_NA;
}


interrupt void read_rotary_encoder(void)
{
    static u1 i;
    u1 new;

    if(INTCONbits.RABIF == 0) return;

    INTCONbits.RABIF = 0;                   // clear int flag
    new = (PORTB >> 6) & 0x03;              /* PORTB:AB00_0000 */
    i = (i << 2) + new;                     /* i:0000_OONN             */
    i &= 15;                                /*        ^  ^-- N:new value */
                                            /*        +----- O:old value */
    switch (i) {
        case 0b00001101:                    /* 3 -> 1 */
            rot = ROT_FF; break;
        case 0b00000111:                    /* 1 -> 3 */
            rot = ROT_RW; break;
    }
}

void main(void)
{
    init();

    volatile u1 out = 0;

    while(1) {
        if(rot == ROT_NA) continue;

        if((rot == ROT_FF) && (out < OUT_MAX)) out++;
        else if((rot == ROT_RW) && (out > 0)) out--;

        PORTC = ~out;                 // output to port c (LO: LED ON)
        rot = ROT_NA;
    }

    return;
}

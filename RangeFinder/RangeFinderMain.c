/*
 * File:   RangeFinderMain.c
 * Author: pnyholm
 *
 * Created on July 25, 2013, 11:32 AM
 */

#include <p24F04KA201.h>
#include "GlobalConfig.h"

#define RANGE_CONST 1000000.0/62500.0/148.0;

float Range;

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0; // clear interrupt flag

    static unsigned short EchoFlag = 0;
    
    // if the pin was low
    if (EchoFlag == 0){
        EchoFlag = 1;   // set the flag
        TMR1 = 0;       // reset the timer
    }
    // else the pin was high
    else {
        EchoFlag = 0;  // set the flag
        Range = TMR1*RANGE_CONST;
    }
}

int main() {

    AD1PCFG = 0b0001110000111111;

    TRISBbits.TRISB7 = 0; // for use as debug LED
    TRISBbits.TRISB4 = 1; // echo input
    TRISAbits.TRISA4 = 0; // trigger output

    ConfigTimer(); // 62.5 kHz

    CNPD1bits.CN1PDE = 1; // configure as pull-down
    CNEN1bits.CN1IE = 1; // enable change notification

    IEC1bits.CNIE = 1; // enable change notification
    IPC4bits.CNIP = 0b111; // highest priority
    IFS1bits.CNIF = 0; // interrupt flag


    while (1){
        // device trigged on falling edge of pulse
        LATAbits.LATA4 = 1;
        TMR1 = 0;
        while(TMR1<10);
        LATAbits.LATA4 = 0;

        // now wait a second to see if we get the cho
        TMR1 = 0;
        while(TMR1<62500);

        // now turn on B7 LED proportional to how far it is in inches
        float OnTime;
        OnTime = Range*100;
        LATBbits.LATB7 = 1;
        TMR1 = 0;
        while(TMR1<OnTime);
        LATBbits.LATB7 = 0;
    }
    return 0;
}


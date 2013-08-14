#include "GlobalConfig.h"

//==============================================================================
// Define Global Variables
//==============================================================================

unsigned long int TIMER1_FREQ = 62500;
unsigned long int STEPPER_FREQ = 62500;

//==============================================================================
// Module Code
//==============================================================================

void configurePins(void)
{
    // put all analog pins in digital mode
    AD1PCFG = 0xFFFF;

    // stepper motor
    TRISBbits.TRISB4 = 0; // pin 9
    TRISAbits.TRISA4 = 0; // pin 10
    TRISAbits.TRISA2 = 0; // pin 7
    TRISAbits.TRISA3 = 0; // pin 8
    TRISBbits.TRISB2 = 0; // pin 6

    // rangefinder
    TRISAbits.TRISA6 = 0; // pin 14
    TRISBbits.TRISB9 = 1; // pin 13
    CNPD2bits.CN21PDE = 1; // configure pin 13 as pull-down
    TRISBbits.TRISB8 = 0; // pin 12
    TRISBbits.TRISB7 = 1; // pin 11
    CNPD2bits.CN23PDE = 1; // configure pin 11 as pull-down

    // infrared beacon
    TRISBbits.TRISB0 = 1; // pin 4
    AD1PCFGbits.PCFG2 = 0; // pin 4 as analog input
    TRISBbits.TRISB1 = 1; // pin 5

    // SPI

    // debug
    TRISAbits.TRISA0 = 0; // pin 2
    TRISAbits.TRISA1 = 0; // pin 3
}

void configureTimers(void)
{
    // make sure watchdog timer is disabled
    RCONbits.SWDTEN = 0;

    // set oscillator post-scaler (divide by 1)
    CLKDIVbits.RCDIV = 0b000;

    //--------------------------------------------------------------------------
    // Configure Timer 1 (LCD timer)
    //--------------------------------------------------------------------------

    T1CONbits.TCKPS = 0b11; // set Timer1 prescale: 1:256
    T1CONbits.TCS = 0; // set Timer1 to use internal clock
    T1CONbits.TGATE = 0; // clear gated time accumulation enable bit
    T1CONbits.TSYNC = 0; // configure synchronous or asynchronous operation

    PR1 = 156; // set timer period

    IEC0bits.T1IE = 0; // enable Timer1 interrupt
    IFS0bits.T1IF = 0; // clear the interrupt flag
    IPC0bits.T1IP = 0b111; // set interrupt priority to highest priority

    T1CONbits.TON = 1; // turn on Timer1

    // TIMER_FREQ = FOSC / N / 2 / Prescale;
    TIMER1_FREQ = 62500;

    //--------------------------------------------------------------------------
    // Configure Timer 2 (Morse timer)
    //--------------------------------------------------------------------------

    T2CONbits.T32 = 0; // disable Timer 2/3 32-bit mode

    T2CONbits.TCKPS = 0b11; // set Timer2 prescale: 1:256
    T2CONbits.TCS = 0; // set Timer2 to use internal clock
    T2CONbits.TGATE = 0; // clear gated time accumulation enable bit

    PR2 = 62500; // set timer period (temporary, equals about 0.18s)

    IEC0bits.T2IE = 0; // enable Timer2 interrupt
    IFS0bits.T2IF = 0; // clear the interrupt flag
    IPC1bits.T2IP = 0b111; // set interrupt priority to highest priority

    T2CONbits.TON = 1; // turn on Timer2

    // TIMER_FREQ = FOSC / N / 2 / Prescale;
    STEPPER_FREQ = 62500;
}

void configureICN(void)
{
    IEC1bits.CNIE = 1; // enable change notification
    IPC4bits.CNIP = 0b111; // highest priority
    IFS1bits.CNIF = 0; // interrupt flag
}
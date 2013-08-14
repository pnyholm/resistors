#include "GlobalConfig.h"

//==============================================================================
// Misc Global Settings
//==============================================================================

_FOSC(OSCIOFNC_ON) // allow pin 8 (RA3) to be used for digital output

//==============================================================================
// Define Global Variables
//==============================================================================

unsigned long int TIMER1_FREQ = 62500;
unsigned long int TIMER2_FREQ = 62500;

//==============================================================================
// Module Code
//==============================================================================

void configurePins(void)
{
    //--------------------------------------------------------------------------
    // set up pins
    //--------------------------------------------------------------------------

    // configure all pins to be in digital mode
    AD1PCFG = 0xFFFF;

    // configure pin 5 to be analog
    AD1PCFGbits.PCFG3 = 0;

    // configure all pins as outputs
    TRISAbits.TRISA0 = 0; // pin 2:  debug out
    TRISAbits.TRISA1 = 0; // pin 3:  debug out
    TRISBbits.TRISB0 = 0; // pin 4:  shooting motor trigger
    TRISBbits.TRISB1 = 1; // pin 5:  color detection LED input
    TRISBbits.TRISB2 = 1; // pin 6:  opto-interrupt
    TRISAbits.TRISA2 = 0; // pin 7:  servo
    TRISAbits.TRISA3 = 0; // pin 8:  carousel stepper switch
    TRISBbits.TRISB4 = 0; // pin 9:  carousel stepper A
    TRISAbits.TRISA4 = 0; // pin 10: carousel stepper B
    TRISBbits.TRISB7 = 0; // pin 11: debug out
    TRISBbits.TRISB8 = 0; // pin 12: debug out
    TRISBbits.TRISB9 = 0; // pin 13: debug out
    TRISAbits.TRISA6 = 0; // pin 14: debug out
    Nop();

    //--------------------------------------------------------------------------
    // set up pins for SPI slave
    //--------------------------------------------------------------------------
    TRISBbits.TRISB14 = 1; // pin 17: SDI1 (SPI input)
    TRISBbits.TRISB13 = 0; // pin 16: SDO1 (SPI output)
    TRISBbits.TRISB12 = 1; // pin 15: SCK1 (SPI clock from master)
    TRISBbits.TRISB15 = 1; // pin 18: SS1  (SPI slave select)
    Nop();
}

void configureICN(void)
{
    IEC1bits.CNIE = 1; // enable change notification
    IPC4bits.CNIP = 0b111; // highest priority
    IFS1bits.CNIF = 0; // interrupt flag
}

void configureTimers(void)
{
    // set oscillator post-scaler (divide by 1)
    CLKDIVbits.RCDIV = 0b000;

    //--------------------------------------------------------------------------
    // Configure Timer 1
    //--------------------------------------------------------------------------

    T1CONbits.TCKPS = 0b11; // set Timer1 prescale: 1:256
    T1CONbits.TCS = 0; // set Timer1 to use internal clock
    T1CONbits.TGATE = 0; // clear gated time accumulation enable bit
    T1CONbits.TSYNC = 0; // configure synchronous or asynchronous operation

    PR1 = 0xffff; // set timer period

    IEC0bits.T1IE = 0; // disable Timer1 interrupt
    IFS0bits.T1IF = 0; // clear the interrupt flag
    IPC0bits.T1IP = 0b111; // set interrupt priority to highest priority

    T1CONbits.TON = 1; // turn on Timer1

    // TIMER_FREQ = FOSC / N / 2 / Prescale;
    TIMER1_FREQ = 62500;

    //--------------------------------------------------------------------------
    // Configure Timer 2 
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
    TIMER2_FREQ = 62500;
}


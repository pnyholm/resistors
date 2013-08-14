#include "ColorDetection.h"

//==============================================================================
// Private function protos
//==============================================================================
int getValue(void);

// BALL names for outputing to ports
#define PURPLEBALL LATAbits.LATA2
#define PINKBALL LATAbits.LATA6
#define WHITEBALL LATAbits.LATA4

// calculate boundaries
#define LOWER 330
#define UPPER 590

void configureADC(void){

    AD1PCFGbits.PCFG3 = 0;

    //AD1CON1
    _ADSIDL = 0b0;  // continue operation in idle mode
    _FORM = 0b00;   // Data output as integer
    _SSRC = 0b111;  // auto-convert
    _ASAM = 0b0;    // sample immediately // *DPK* switched to manual sampling
    _SAMP = 0;      // 0=holding, 1=sampling
                    // _DONE 1=completed, 0=notCompleted

    //AD1CON2
    _VCFG = 0b010;   // Vr+ = Vdd, Vr- = Vss
                     // _OFFCAL
    _CSCNA = 0b0;    // dont scan inputs
                     // _BUFS
    _SMPI = 0b01000; // interrupt after 16th sample/convert seq
    _BUFM = 0b0;     // configured as one 16-word buffer
    _ALTS = 0b0;     // always use Sample A inputs
    _OFFCAL = 0;

    //AD1CON3
    _ADRC = 0b0;        // derive clock from system
                        // _EXTSAM
    _SAMC = 0b00111;    // Auto-Sample Time
    _ADCS = 0b001111;   // A/D Conversion Clock Period

    //AD1CHS
    _CH0NA = 0b0;       // neg mexA input is 0
    _CH0SA = 0b0011;    // pos muxA input is AN3
    _CH0NB = 0b0;
    _CH0SB = 0b0010;

    AD1CSSL = 0x0000;// channel scanning disabled

    IEC0bits.AD1IE = 0; // turn off interrupt
    IFS0bits.AD1IF = 0; // clear interrupt flag

    _ADON = 0b1;
}

int getValue(void){
    _SAMP = 1;          // request an ADC read
    while (_SAMP) {}    // wait for automatic conversion
    return ADC1BUF0;    // return the value as an integer
}

enum BallColor detectColor(void){
    int ADC_OUT = getValue();

    if(ADC_OUT < LOWER)
    {
        return PURPLE;
    }
    else if(ADC_OUT < UPPER)
    {
        return PINK;
    }
    else
    {
        return WHITE;
    }
}
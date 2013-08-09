/***********************************************************************
 *  Color Detector
 *  Ali Wood
 *  MEEn 495R
 ***********************************************************************/
#include <p24F04KA201.h>
#include <stdlib.h>

// BALL names for outputing to ports

#define PURPLEBALL LATAbits.LATA2
#define PINKBALL LATAbits.LATA6
#define WHITEBALL LATAbits.LATA4

// set system clock to Fast RC Oscillator
_FOSCSEL(FNOSC_FRCPLL);

_FOSC(OSCIOFNC_ON);

// calculate boundaries
static const int ADC_MAX = 1023;
static const int LOWER = 341;
static const int UPPER = 475;

void initColorDetection(void);
void init_ADC(void);
void configTimer(void);
int ADC_Value(void);
void DetectColor(void);

int main()
{
    initColorDetection();
    configTimer();      // configure timer
    init_ADC();

    while(1) {
        DetectColor();
    }
    return 0;
}

void initColorDetection(){
    // set debug pins as outputs

    TRISAbits.TRISA2 = 0;       //pin 7 is purple out
    TRISAbits.TRISA6 = 0;       //pin 14 is pink out
    TRISAbits.TRISA4 = 0;       //pin 10 is white out

    TRISBbits.TRISB1 = 1;        //pin 5 is in
}

void init_ADC(){

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
    _CH0NA = 0b0;  // neg mexA input is 0
    _CH0SA = 0b0011;// pos muxA input is AN3
    _CH0NB = 0b0;
    _CH0SB = 0b0010;

    AD1CSSL = 0x0000;// channel scanning disabled

    IEC0bits.AD1IE = 0; //turn off interupt
    IFS0bits.AD1IF = 0;

    _ADON = 0b1;
}

void configTimer() {
    _RCDIV = 0b000;     // clock divide.  Normal scaling

    T2CONbits.TCKPS = 0b10; // set prescale value 1:64
    T2CONbits.TON = 1; // enable timer
    _T2IE = 0;
    _T2IF = 0;

    TMR2 = 0; // clear timer
}

int ADC_Value(){

    _SAMP = 1;
    while (_SAMP);

    return ADC1BUF0;
}

void DetectColor(){
    int ADC_OUT = ADC_Value();

    PURPLEBALL = 0;
    PINKBALL = 0;
    WHITEBALL = 0;

    if(ADC_OUT < 330)
    {
       PURPLEBALL = 1;
       PINKBALL = 0;
       WHITEBALL = 0;
    }
    else if(ADC_OUT < 590)
    {
       PURPLEBALL = 0;
       PINKBALL = 1;
       WHITEBALL = 0;
    }
    else
    {
       PURPLEBALL = 0;
       PINKBALL = 0;
       WHITEBALL = 1;
    }
}

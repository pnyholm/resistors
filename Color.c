/***********************************************************************
 *  Color Detector
 *  Ali Wood
 *  MEEn 495R
 ***********************************************************************/
#include <p24F04KA201.h>
#include <stdlib.h>

// set system clock to Fast RC Oscillator
_FOSCSEL(FNOSC_FRCPLL);

// BALL names for outputing to ports
#define PURPLEBALL _RA2
#define PINKBALL _RA6
#define WHITEBALL _RA4

// calculate boundaries
static const int ADC_MAX = 1023;
 int Vref; //~2000 mV
static const int LOWER = 80;    //in mV
static const int UPPER = 110;   //in mV

void initColorDetection(void);
void init_ADC(void);
void configTimer(void);
int ADC_Avg(void);
void DetectColor(void);

int main() {
    _AD1IE = 0;         //turn off interupt flag
    _TON = 0;
    configTimer();      // configure timer
    _TON = 1;
    _RCDIV = 0b000;     // clock divide.  Normal scaling
    //_LATA2 = 0;       // clear yellow input latch
    _OCTSEL = 0;        // select TIMER2 as clock source

    init_ADC();
    initColorDetection();

    while(1) {
        DetectColor();
    }
    return 0;
}

initColorDetection(){
    // set debug pins as outputs
    _TRISA2 = 0;       //pin 7 is purple out
    _TRISA6 = 0;       //pin 8 (14) is pink out
    _TRISA4 = 0;       //pin 10 is white out
}

init_ADC(){
    AD1PCFG = 0xFFF7; //0xFFFB; // sample only AN2 voltage

    //AD1CON1
    _ADON = 0b1;
    _ADSIDL = 0b0;
    _FORM = 0b00;  // Data output as integer
    _SSRC = 0b111; // auto-convert
    _ASAM = 0b1;   // sample immediately

    //AD1CON2
    _VCFG = 0b001;  // Vr+ (Vref+ = 2V), Vr- AVss
    _CSCNA = 0b0;   // use CH0SA bits as mux inputs
    _BUFS = 0b1;    // filling F8-FF, user access F0-F7
    _SMPI = 0b1111; // interrupt after 16th sample/convert seq
    _BUFM = 0b0;    // configured as one 16-word buffer
    _ALTS = 0b0;    // always use MUXA

    //AD1CON3 = 0x0D09; // AD Clock as derivative of system clock, 13TAD,16 samples are collected in 1mSec,Fcy =4Mhz 1Tcy=0.26uS
    _ADRC = 0b0;        // derive clock from system
    _SAMC = 0b00111;    // Auto-Sample Time
    _ADCS = 0b00011111; // A/D Conversion Clock Period

    // AD1CHS  = 0x0005; // Positive sample input channel for MUX A to use AN2,Negative input channel for MUX A to use VR-
    _CH0NA = 0b0;   // neg mexA input is 0
    _CH0SA = 0b0011;// pos muxA input is AN2


    AD1CSSL = 0x0000; // channel scanning disabled
}

configTimer() {
    T2CONbits.TCKPS = 0b10; // set prescale value 1:64
    TMR2 = 0; // clear timer
}

ADC_Avg(){
    int sum = ADC1BUF0 +
              ADC1BUF1 +
              ADC1BUF2 +
              ADC1BUF3 +
              ADC1BUF4 +
              ADC1BUF5 +
              ADC1BUF6 +
              ADC1BUF7 +
              ADC1BUF8 +
              ADC1BUF9 +
              ADC1BUFA +
              ADC1BUFB +
              ADC1BUFC +
              ADC1BUFD +
              ADC1BUFE +
              ADC1BUFF;

    int avg = sum/16;
    return avg;
}

DetectColor(){
    Vref = 2000;//_LATA0;

    int ADC_OUT = ADC_Avg();
    int upBound = UPPER*ADC_MAX/Vref;
    int lowBound = LOWER*ADC_MAX/Vref;

    if(ADC_OUT < lowBound){
        PURPLEBALL = 1;
        PINKBALL = 0;
        WHITEBALL = 0;
    }
    else if(lowBound < ADC_OUT < upBound){
        PURPLEBALL = 0;
        PINKBALL = 1;
        WHITEBALL = 0;
    }
    else { //if(upBound < ADC_OUT){
        PURPLEBALL = 0;
        PINKBALL = 0;
        WHITEBALL = 1;
    }
}

/***********************************************************************
 *  Color Detector
 *  Ali Wood
 *  MEEn 495R
 ***********************************************************************/
#include <p24F16KA301.h>
#include <stdlib.h>

// set system clock to Fast RC Oscillator
_FOSCSEL(FNOSC_FRCPLL);

// BALL names for outputing to ports
#define PURPLEBALL _RA2
#define PINKBALL _RA6
#define WHITEBALL _RA4//_LATA4

// calculate boundaries
static const int ADC_MAX = 4095;
int Vref; //~2000 mV
static const int LOWER = 80;    //in mV
static const int UPPER = 110;   //in mV
int test = 0;

void initColorDetection(void);
void init_ADC(void);
void configTimer(void);
int ADC_Avg(void);
void DetectColor(void);

int main() {
    _AD1IE = 0;         //turn off interupt

    _TON = 0;           // disable timer (for config) TODO: move to configTimer()
    configTimer();      // configure timer
    _RCDIV = 0b000;     // clock divide.  Normal scaling TODO: move to configTimer()
    _TON = 1;           // enable timer TODO: move to configTimer()

    //_LATA2 = 0;       // clear yellow input latch
    //_OCTSEL = 0;        // select TIMER2 as clock source

    init_ADC();
    initColorDetection();

    while(1) {

        DetectColor();
        /*
        PURPLEBALL = 0;
        PINKBALL = 0;
        WHITEBALL = 0;
        int color = DetectColor();
        if(color == 0)
            PURPLEBALL = 1;
        else if(color == 1)
            PINKBALL = 1;
        else if(color == 2)
            WHITEBALL = 1;
         */
    }
    return 0;
}

void initColorDetection(){
    // set debug pins as outputs
    _TRISA2 = 0;       //pin 7 is purple out
    _TRISA6 = 0;       //pin 14 is pink out
    _TRISA4 = 0;       //pin 10 is white out

    _TRISB2 = 1;        //pin 6 is in
    _TRISB1 = 1;        //pin 5 is in

}

void init_ADC(){

    ANSBbits.ANSB2 = 0b1; // pin 6 analog
    ANSBbits.ANSB1 = 0b1;     // pin 5 is analog input

    //AD1CON1
    _ADON = 0b1;
    _ADSIDL = 0b0;  // continue operation in idle mode
    _MODE12 = 0b1;  // 12-bit A/D operation
    _FORM = 0b00;   // Data output as integer
    _SSRC = 0b0111; // auto-convert
    _ASAM = 0b0;    // sample immediately // *DPK* switched to manual sampling
                    // _SAMP 0=holding, 1=sampling
                    // _DONE 1=completed, 0=notCompleted

    //AD1CON2
    _PVCFG = 0b01;  // Vr+ = external Vref+, 2V
    _NVCFG = 0b0;   // Vr- = Vss
                    // _OFFCAL
    _BUFREGEN = 0b0;// treat buffer as FIFO
    _CSCNA = 0b0;   // dont scan inputs
                    // _BUFS
    _SMPI = 0b01000;// interrupt after 16th sample/convert seq
    _BUFM = 0b0;    // configured as one 16-word buffer
    _ALTS = 0b0;    // always use Sample A inputs

    //AD1CON3
    _ADRC = 0b0;        // derive clock from system
                        // _EXTSAM
    _SAMC = 0b00111;    // Auto-Sample Time
    _ADCS = 0b00011111; // A/D Conversion Clock Period

    //AD1CON5
    _ASEN = 0b0;    // disable auto-scan
    _LPEN = 0b0;    // fullpower mode after scan
    _BGREQ = 0b0;   // band gap not enabled
    _ASINT = 0b00;  // no interrupt (threshold detection)
    _WM = 0b00;     // legacy operation

    //AD1CHS
    _CH0NA = 0b000;  // neg mexA input is 0
    _CH0SA = 0b00100;// pos muxA input is AN4        

    AD1CSSL = 0x0000;// channel scanning disabled
}

void configTimer() {
    T2CONbits.TCKPS = 0b10; // set prescale value 1:64
    TMR2 = 0; // clear timer
}

int ADC_Avg(){
    // sample 18 times to fill buffer
    unsigned short int i;
    for (i = 0; i < 18; i++)
    {
        _SAMP = 1;
//        while (!_DONE);
        while (_SAMP);
    }
    // average all measurements in the buffer and return result
    long int sum = ADC1BUF0 +
              ADC1BUF1 +
              ADC1BUF2 +
              ADC1BUF3 +
              ADC1BUF4 +
              ADC1BUF5 +
              ADC1BUF6 +
              ADC1BUF7 +
              ADC1BUF8 +
              ADC1BUF9 +
              ADC1BUF10 +
              ADC1BUF11 +
              ADC1BUF12 +
              ADC1BUF13 +
              ADC1BUF14 +
              ADC1BUF15 +
              ADC1BUF16 +
              ADC1BUF17;

    int avg = sum/18;
    return avg;
}

void DetectColor(){
    Vref = 1900;
    int ADC_OUT = ADC_Avg();
    int upBound = UPPER*ADC_MAX/Vref;
    int lowBound = LOWER*ADC_MAX/Vref;

    if(ADC_OUT < 850){
       PURPLEBALL = 1;
       PINKBALL = 0;
       //WHITEBALL = 0;
    }
    else if(ADC_OUT >= 850 && ADC_OUT < 900){
       PURPLEBALL = 0;
       PINKBALL = 1;
       //WHITEBALL = 0;
    }
    else if(ADC_OUT >= 900){
       PURPLEBALL = 0;
       PINKBALL = 0;
      // WHITEBALL = 1;
    }
    //return ballColor;

}

#include "ConfigureSPI.h"

void InitSPISlave(){
    TRISBbits.TRISB15 = 1;      // SS1
    TRISBbits.TRISB14 = 1;      // SDI1
    TRISBbits.TRISB13 = 0;      // SDO1
    TRISBbits.TRISB12 = 1;      // SCK1
}

// function to configure SPI module for the STANDARD SLAVE mode
void configureSlaveSPI(){

    // clear SPIEN
    SPI1STATbits.SPIEN=0;

    // clear the SPIBUF register
    SPI1BUF = 0;

    // if using interrupts
    IFS0bits.SPI1IF = 0;        // clear the interrupt flag
    IEC0bits.SPI1IE = 0;        // enable interrupt
    IPC2bits.SPI1IP = 0b111;      // set the interrupt priority

    // write the desired settings to SPI1CON1 and SPI1CON2 (MSTEN = 0 for master)
    SPI1CON1bits.DISSCK = 0;    // disable SCK1 pin bit (SPI Master modes only)
    SPI1CON1bits.DISSDO = 0;    // disables SDO1 pin bit
    SPI1CON1bits.MODE16 = 0;    // word/byte communication select bit

    // clear the SMP bit
    SPI1CON1bits.SMP = 0;       // SPI1 data input sample phase bit
    SPI1CON1bits.CKE = 0;       // SPI1 clock edge select bit

    // if the CKE bit is set, then the SSEN bit must be set to enable the SS1 bit
    SPI1CON1bits.SSEN = 1;      // slave select enable bit (slave mode)
    SPI1CON1bits.CKP = 0;       // clock polarity set bit
    InitSPISlave();
    SPI1CON1bits.MSTEN = 0;     // master mode enable bit
//    SPI1CON1bits.SPRE = 000;    // secondary prescale bits (master mode)
//    SPI1CON1bits.PPRE = 00;     // primary prescale bits (master mode)

    SPI1CON2bits.FRMEN = 0;     // frame SPI1 support bit
    SPI1CON2bits.SPIFSD = 1;    // frame sync pulse direction control on SS1 pin bit
    SPI1CON2bits.SPIFPOL = 0;   // frame sync pulse polarity bit (frame mode only)
    SPI1CON2bits.SPIFE = 0;     // frame sync pulse edge select bit
    SPI1CON2bits.SPIBEN = 0;    // enhanced buffer enable bit

    // clear the SPIROV bit
    SPI1STATbits.SPIROV = 0;    // recieve overflow flag bit

    // enable SPI operation by setting the SPIEN bit
    SPI1STATbits.SPIEN = 1;     // SPI1 enable bit
}

unsigned short ReadSPI(void)
{
    unsigned short data;

    // wait until transmit buffer is empty
    while(SPI1STATbits.SPITBF!=0);
    // write bogus data
//    SPI1BUF=0b11111111;
    // wait until transmit buffer is empty
//    while(SPI1STATbits.SPITBF!=0);

    // wait until the recieve buffer is full
    while(SPI1STATbits.SPIRBF==0);
    // when the recieve buffer is not empty anymore, read what is in the buffer
    data=SPI1BUF;
//    LATBbits.LATB7 = 1; // toggle LED
    // wait until the recieve complete flag is set
//    while(SPI1STATbits.SPIRBF!=0);

  return(data);
}

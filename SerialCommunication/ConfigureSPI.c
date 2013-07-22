#include "ConfigureSPI.h"

// function to configure SPI module for STANDARD MASTER mode
void ConfigureMasterSPI(){

    // if using interrupts
    IFS0bits.SPI1IF = 0;        // clear the interrupt flag
    IEC0bits.SPI1IE = 1;        // enable interrupt
    IPC2bits.SPI1IP = 111;      // set the interrupt priority

    // write the desired settings to SPI1CON1 and SPI1CON2 (MSTEN = 1 for master)
    SPI1CON1bits.DISSCK = ;     // disable SCK1 pin bit (SPI Master modes only)
    SPI1CON1bits.DISSDO = ;     // disables SDO1 pin bit
    SPI1CON1bits.MODE16 = ;     // word/byte communication select bit
    SPI1CON1bits.SMP = ;        // SPI1 data input sample phase bit
    SPI1CON1bits.CKE = ;        //SPI1 clock edge select bit
    SPI1CON1bits.SSEN = 0;      // slave select enable bit (slave mode)
    SPI1CON1bits.CKP = ;        // clock priority set bit
    SPI1CON1bits.MSTEN = 1;     // master mode enable bit
    SPI1CON1bits.SPRE = ;       // secondary prescale bits (master mode)
    SPI1CON1bits.PPRE = ;       // primary prescale bits (master mode)

    SPI1CON2bits.FRMEN = ;      // frame SPI1 support bit
    SPI1CON2bits.SPIFSD = ;     // frame sync pulse direction control on SS1 pin bit
    SPI1CON2bits.SPIFPOL = ;    // frame sync pulse polarity bit (frame mode only)
    SPI1CON2bits.SPIFE = ;      // frame sync pulse edge select bit
    SPI1CON2bits.SPIBEN = ;     // enhanced buffer enable bit

    // clear the SPIROV bit
    SPI1STATbits.SPIROV = 0;     // recieve overflow flag bit

    // enable SPI operation by setting the SPIEN bit
    SPI1STATbits.SPIEN = 1;     // SPI1 enable bit

    // write the data to be transmitted to the SPI1BUF register.
    // transmission (and reception) will start as soon as data is written to the SPI1BUF register.
}

// function to configure SPI module for the STANDARD SLAVE mode
void ConfigureSlaveSPI(){

    // clear the SPIBUF register
    SPI1BUF = 0; // is this right?

    // if using interrupts
    IFS0bits.SPI1IF = 0;        // clear the interrupt flag
    IEC0bits.SPI1IE = 1;        // enable interrupt
    IPC2bits.SPI1IP = 111;      // set the interrupt priority

    // write the desired settings to SPI1CON1 and SPI1CON2 (MSTEN = 0 for master)
    SPI1CON1bits.DISSCK = ;     // disable SCK1 pin bit (SPI Master modes only)
    SPI1CON1bits.DISSDO = ;     // disables SDO1 pin bit
    SPI1CON1bits.MODE16 = ;     // word/byte communication select bit

    // clear the SMP bit
    SPI1CON1bits.SMP = 0;        // SPI1 data input sample phase bit
    SPI1CON1bits.CKE = ;        //SPI1 clock edge select bit

    // if the CKE bit is set, then the SSEN bit must be set to enable the SS1 bit
    SPI1CON1bits.SSEN = 0;      // slave select enable bit (slave mode)
    SPI1CON1bits.CKP = ;        // clock priority set bit
    SPI1CON1bits.MSTEN = 0;     // master mode enable bit
    SPI1CON1bits.SPRE = ;       // secondary prescale bits (master mode)
    SPI1CON1bits.PPRE = ;       // primary prescale bits (master mode)

    SPI1CON2bits.FRMEN = ;      // frame SPI1 support bit
    SPI1CON2bits.SPIFSD = ;     // frame sync pulse direction control on SS1 pin bit
    SPI1CON2bits.SPIFPOL = ;    // frame sync pulse polarity bit (frame mode only)
    SPI1CON2bits.SPIFE = ;      // frame sync pulse edge select bit
    SPI1CON2bits.SPIBEN = ;     // enhanced buffer enable bit

    // clear the SPIROV bit
    SPI1STATbits.SPIROV = 0;     // recieve overflow flag bit

    // enable SPI operation by setting the SPIEN bit
    SPI1STATbits.SPIEN = 1;     // SPI1 enable bit
}

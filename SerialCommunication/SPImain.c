/* 
 * File:   SPImain.c
 * Author: pnyholm
 *
 * Created on July 19, 2013, 4:14 PM
 */

#include <p24F04KA201.h>

int main() {

    char master_flag;
    master_flag = 0;      // 1 is master, 0 is slave

    AD1PCFG = 0b0001110000111111;

    TRISBbits.TRISB7 = 0; // for use as debug LED

    TRISAbits.TRISA4 = 0; // for use as debug LED
    TRISBbits.TRISB4 = 0; // for use as debug LED

    ConfigTimer();

    switch (master_flag) {
        case 0:
            //slave
            ConfigureSlaveSPI();
            break;

        case 1:
            //master
            ConfigureMasterSPI();
            break;
    }

    unsigned short DataRX;

    while (1){
        switch (master_flag) {
            case 0:
                // if you get anything from ReadSPI toggle a debug LED
                DataRX = ReadSPI();
//                DataRX = ~DataRX;
                LATAbits.LATA4 = (DataRX & 0b000000010) != 0;
                LATBbits.LATB4 = (DataRX & 0b000000001) != 0;

                if (DataRX == 0b10101010) {
                    LATBbits.LATB7 ^= 1; // toggle LED
                }
                break;

            case 1:
                TMR1 = 0;
                while (TMR1 < 50000);
                WriteSPI(0b10101010);   // write a byte
                LATBbits.LATB7 = 1;     // turn LED on
                TMR1 = 0;               // reset timer
                while (TMR1 < 1000);    // wait for a small time (so I can see LED blink)

//                WriteSPI(0b11000011);   // write another byte
//                LATBbits.LATB7 = 0;     // turn LED off
//                TMR1 = 0;               // reset timer
//                while (TMR1 < 1000);    // wait for a small time (so I can see LED blink)
                while(1);
                break;
        }
    };
    return 0;
}


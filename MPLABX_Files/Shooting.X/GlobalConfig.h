/* 
 * File:   GlobalConfig.h
 * Author: Daniel Koch
 *
 * Created on July 15, 2013, 10:47 AM
 */

#ifndef GLOBALCONFIG_H
#define	GLOBALCONFIG_H

#include <p24F04KA201.h>

//==============================================================================
// Pin Assignments
//==============================================================================
#define DEBUG_1         LATAbits.LATA0 // pin 2
#define DEBUG_2         LATAbits.LATA1 // pin 3
#define TRIGGER         LATBbits.LATB0 // pin 4
#define COLOR_DETECT    PORTBbits.RB2  // pin 5
#define OPTO_INT        PORTBbits.RB2  // pin 6
#define OPTO_E           CNEN1bits.CN6IE // enable change notification for pin 6
#define SERVO           LATAbits.LATA2 // pin 7
#define SM_E            LATAbits.LATA3 // pin 8
#define SM1_B           LATBbits.LATB4 // pin 9
#define SM1_A           LATAbits.LATA4 // pin 10
#define DEBUG_3         LATBbits.LATB7 // pin 11
#define DEBUG_4         LATBbits.LATB8 // pin 12
#define DEBUG_5         LATBbits.LATB9 // pin 13
#define DEBUG_6         LATAbits.LATA6 // pin 14

void configurePins(void);

void configureICN(void);

//==============================================================================
// Timer Configuration
//==============================================================================

void configureTimers(void);

#endif	/* GLOBALCONFIG_H */


/* 
 * File:   GlobalConfig.h
 * Author: Daniel Koch
 *
 * Created on August 8, 2013, 10:02 PM
 */

#ifndef GLOBALCONFIG_H
#define	GLOBALCONFIG_H

#include <p24F04KA201.h>

//==============================================================================
// Pin Assignments
//==============================================================================

// stepper motor
#define SM1_A LATBbits.LATB4
#define SM1_B LATAbits.LATA4
#define SM2_A LATAbits.LATA2
#define SM2_B LATAbits.LATA3
#define SM_E LATBbits.LATB2

// rangefinders
#define RF1_TRIG LATAbits.LATA6
#define RF1_ECHO PORTBbits.RB9
#define RF1_E CNEN2bits.CN21IE

#define RF2_TRIG LATBbits.LATB8
#define RF2_ECHO PORTBbits.RB7
#define RF2_E CNEN2bits.CN23IE

// infrared beacon
#define IR_AMPLITUDE PORTBbits.RB0
#define IR_FREQUENCY PORTBbits.RB1

// SPI

// debug
#define DEBUG1 LATAbits.LATA0
#define DEBUG2 LATAbits.LATA1

void configurePins(void);

//==============================================================================
// Timer Configuration
//==============================================================================

#define TIMER1 TMR1
#define TIMER1_PERIOD PR1
extern unsigned long int TIMER1_FREQ;

#define STEPPER_TIMER TMR2
#define STEPPER_PERIOD PR2
extern unsigned long int STEPPER_FREQ;

void configureTimers(void);

//==============================================================================
// Input Change Notification Configuration
//==============================================================================

void configureICN(void);

#endif	/* GLOBALCONFIG_H */


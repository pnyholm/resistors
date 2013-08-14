/*
 * File:   main.c
 * Author: Daniel Koch
 *
 * Created on July 11, 2013, 10:56 AM
 */

#include <stdlib.h>
#include <p24F04KA201.h>
#include "GlobalConfig.h"
#include "StepperMotors.h"
#include "RangeFinders.h"

//==============================================================================
// Initial Configuration
//==============================================================================

// set system clock to fast RC oscilator with 4X PLL (~32 MHz)
_FOSCSEL(FNOSC_FRCPLL);

// turn watchdog timer (WDT) off
_FWDT(FWDTEN_OFF);

// set OSCO pin for use as I/O
_FOSC(OSCIOFNC_ON);

//==============================================================================
// State Machine States
//==============================================================================

enum States { Waiting,
              WaitingForNewChar,
              High,
              Low,
              CalWaiting,
              CalHigh };

//==============================================================================
// Event Checkers
//==============================================================================



/*
 *
 */
int main()
{
    // initialize
    configurePins();
    configureTimers();
    configureICN();

    //DEBUG1 = 1;
    //DEBUG2 = 1;

    // test stepper motors
    //testStepperMotors(HIGH);

    initRangeFinders();

    while (1) { testRangeFinders(); }

    return (EXIT_SUCCESS);
}

//==============================================================================
// Interrupts
//==============================================================================

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF = 0; // clear interrupt flag

    if (RF_E)
    {
        if (RF_ECHO_FLAG == 0)
        {
            RF_ECHO_FLAG = 1; // set the flag
            TIMER1 = 0; // reset the timer
        }
        else
        {
            RF_ECHO_FLAG = 0; // set the flag
            RF_RANGE = TIMER1; // record the high time of the return pulse
            RF_READ_FLAG = 1;
        }
    }
}
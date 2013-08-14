/* 
 * File:   main.c
 * Author: The Resistors
 *
 * Created on August , 2013, 10:56 PM
 */

#include <stdlib.h>
#include <p24F04KA201.h>
#include "GlobalConfig.h"
#include "Servo.h"
#include "StepperMotors.h"
#include "ColorDetection.h"
#include "ConfigureSPI.h"

void OptoCheckForRise(char);
char IncrementPosition(void);
void FireBall(void);

char OPTO_FLAG = 0;
short CURRENT_POSITION;
short COUNT_TURNS;

char TARGET_RECIEVED = 0;

enum BallColor TARGET_COLOR;

//==============================================================================
// Initial Configuration
//==============================================================================

// turn watchdog timer (WDT) off
_FWDT(FWDTEN_OFF);

// set system clock to fast RC oscilator with 4X PLL (~32 MHz)
_FOSCSEL(FNOSC_FRCPLL);

//==============================================================================
// State Machine States
//==============================================================================

enum States { HOMING, SORTING, HOLD_FIRE, FIRE};

// initialize current state to homing


//==============================================================================
// Event Checkers
//==============================================================================


/*
 * 
 */
int main() {

    // make sure watchdog timer is disabled
    RCONbits.SWDTEN = 0;

    // initialize
    configurePins();
    configureTimers();
    configureICN();
    configureADC();
    configureSlaveSPI();
    

    OPTO_E = 1; // intialize change notification for optointerrupt

    // global to keep track of the current position
    short CURRENT_POSITION;

    // initialize counter for balls detected in sorting
    short balls_detected = 0;

    // create empty arrays for storing ball locations (made max possible ball size incase there are errors detecting color)
    // initialize all values to zero
    short purple_balls[6] = {0};
    short pink_balls[6] = {0};
    short white_balls[6] = {0};

    short TARGET_LOCATIONS[6] = {0};
    short num_targets = 0;

    // counters for number of each color of ball
    short num_purple = 0;
    short num_pink = 0;
    short num_white = 0;

    // variable to store the current color of the ball
    enum BallColor color_detected;

    // variable to store data recieved by SPI
    unsigned short DataRX;

    enum States CurrentState = HOMING;
    while (1) {
    // state machine
        switch (CurrentState) {

            case HOMING:
                // spin carousel, stop spinning when the opto-interrupt is triggered
                enableStepperMotors();  // enable the motors
                while (!OPTO_FLAG) {  // while the OPTO_FLAG is not set
                    stepMotor(FORWARD, LOW, 1); // turn the carousel one step at a time
                }

                // call the position we are in POSITION0
                CURRENT_POSITION = 0;

                // set CurrentState to SORTING
                CurrentState = SORTING;

                // set balls_detected to 0
                balls_detected = 0;

                SM_E = 0; // enable stepper
                
                break;

            case SORTING:

                SM_E = 1; // enable stepper
                // if:  the counter is less than 5
                if (balls_detected < 5) {

                    // detect the color of the ball
                    color_detected = detectColor();
                    SM_E = 0;
                    // store what color the ball is in proper array
                    if (color_detected == PURPLE){
                        purple_balls[num_purple] = CURRENT_POSITION; // store location of purple ball
                        num_purple = num_purple + 1; // increment number of purple balls
                    }
                    else if (color_detected == PINK){
                        pink_balls[num_pink] = CURRENT_POSITION; // store location of pink ball
                        num_pink = num_pink + 1; // increment number of pink balls
                    }
                    else if (color_detected == WHITE) {
                        white_balls[num_white] = CURRENT_POSITION; // store location of white ball
                        num_white = num_white + 1; // increment number of white balls
                    }
                    
                    // increment the number of balls detected counter
                    balls_detected = balls_detected + 1;

                    // rotate to the next ball
                    while (!OPTO_FLAG) {  // while the OPTO_FLAG is not set
                        stepMotor(FORWARD, LOW, 1); // turn the carousel one step at a time
                    }

                    // increment position
                    char okay = IncrementPosition();
                    if (!okay)
                        CurrentState = HOMING;
                }
                // else: we have already stored all six balls
                else {
                // set CurrentState to HOLD_FIRE
                    CurrentState = HOLD_FIRE;
                }
                
                break;

            case HOLD_FIRE:

                // wait until recieve a message from the Master over SPI
                // when recieve the message save it to global TARGET_COLOR
                TARGET_RECIEVED = 0;
                while(!TARGET_RECIEVED){
                    DataRX = ReadSPI();
                    if (DataRX != 0b11111111) {
                        if (DataRX == 0b00001111) {
                            TARGET_COLOR = PURPLE;
                        }
                        else if (DataRX == 0b11110000) {
                            TARGET_COLOR = PINK;
                        }
                        else if (DataRX == 0b00111100) {
                            TARGET_COLOR = WHITE;
                        }
                        TARGET_RECIEVED = 1;
                    }
                }
                
                // set CurrentState to FIRE
                CurrentState = FIRE;
                break;

            case FIRE:
                // if:  the target ball exists
                if (TARGET_COLOR == PURPLE && num_purple != 0) {
                    num_targets = num_purple;
                    short i = 0;
                    for ( i=0; i < num_purple; i = i+1){
                        TARGET_LOCATIONS[i] = purple_balls[i];
                    }
                }
                else if (TARGET_COLOR == PINK && num_pink != 0) {
                    num_targets = num_pink;
                    short i = 0;
                    for ( i=0; i < num_pink; i = i+1){
                        TARGET_LOCATIONS[i] = pink_balls[i];
                    }
                }
                else if (TARGET_COLOR == WHITE && num_white != 0) {
                    num_targets = num_white;
                    short i = 0;
                    for ( i=0; i < num_white; i = i+1){
                        TARGET_LOCATIONS[i] = white_balls[i];
                    }
                }
                else {
                    CurrentState = HOMING;
                    break;
                }

                SM_E = 1; // enable motors
                short num_turns;
                short i = 0;

                // for each of the targets...
                for (i = 0; i < num_targets; i = i + 1) {

                    // calculate the number of turns we need to make
                    num_turns = TARGET_LOCATIONS[i] - CURRENT_POSITION;

                    // if the number of turns needed is less than 0, go backward
                    if (num_turns < 0) {       
                        COUNT_TURNS = 0;
                        while (COUNT_TURNS != num_turns){
                            stepMotor(REVERSE, LOW, 1);
                        }
                        FireBall();
                    }
                    // if turns needed is greater than 0, go forward
                    else if (num_turns >= 0) {
                        COUNT_TURNS = 0;
                        while (COUNT_TURNS != num_turns){
                            stepMotor(FORWARD, LOW, 1);
                        }
                        FireBall();
                    }
                    else if (num_turns == 0) {
                        FireBall();
                    }
                }

                // cut power to carousel
                SM_E = 0;

                //  TODO: send message to Master saying we're finished firing
                
                // set CurrentState to HOLD_FIRE
                CurrentState = HOLD_FIRE;
                break;
        }
    }

    return (EXIT_SUCCESS);
}

//==============================================================================
// Interrupts
//==============================================================================

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    IFS1bits.CNIF = 0; // clear interrupt flag

    OptoCheckForRise(!OPTO_INT);

}

void OptoCheckForRise(char CURRENT_STATE){
    static char LAST_STATE = 0;
    if (LAST_STATE == 0 && CURRENT_STATE == 1) {
        OPTO_FLAG = 1;

    }
    else {
        OPTO_FLAG = 0;
    }
    LAST_STATE = CURRENT_STATE;
    COUNT_TURNS = COUNT_TURNS + 1;
}

char IncrementPosition(void){
    if (CURRENT_POSITION < 4){
        CURRENT_POSITION = CURRENT_POSITION + 1;
    }
    else if (CURRENT_POSITION == 4){
        CURRENT_POSITION = 0;
    }
    else {
        CURRENT_POSITION = 0;
        return 0;
    }
    return 1;
}

void FireBall(void){
    // start the firing motors
    TRIGGER = 1;

    // open and close hatch
    SERVO = 0;
    int result = moveOut();
    if(result == 1){
       moveIn();
    }

    // kill motors
    TRIGGER = 0;
}
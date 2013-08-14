#include "Servo.h"

static const unsigned long HOLD = 50;
static const unsigned long PERIOD = 1250;

void delayServo(int time);

int moveOut(void){
    unsigned long i=0;
    while(i < HOLD){
        delayServo(PERIOD*0.896); //(4000);    // wait for rising edge
        SERVO = 1;          // pulse high
        delayServo(PERIOD*0.104);  //(200);     // wait for falling edge
        SERVO = 0;
        i++;
    }
    return 1;
}

int moveIn(void){
    unsigned long i=0;
    while(i < HOLD){
        delayServo(PERIOD*0.92); //(3780);    // wait for rising edge
        SERVO = 1;          // pulse high
        delayServo(PERIOD*0.08);  //(420);     // wait for falling edge
        SERVO = 0;          // go low
        i++;
    }
    return 1;
}

void delayServo(int time){
    TMR1 = 0;
    while (TMR1 < time);

//    int i=0;
//    while(i<time)
//        i++;
}
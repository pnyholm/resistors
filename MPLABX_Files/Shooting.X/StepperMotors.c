#include "StepperMotors.h"

//==============================================================================
// Private Module Constants
//==============================================================================

static const unsigned int num_steps = 4;

static const char STEP_A[] = {1, 1, 0, 0};
static const char STEP_B[] = {0, 1, 1, 0};

static const unsigned int LOW_PERIOD = 1000;//625; // 100 Hz
static const unsigned int HIGH_PERIOD = 142; // 440 Hz

//==============================================================================
// Private Module Variables
//==============================================================================

unsigned int step1 = 0;

//==============================================================================
// Private Module Function Prototypes
//==============================================================================

enum StepDirection { INCREMENT, DECREMENT };
void advanceStep(unsigned int *step, enum StepDirection direction);

void delay(unsigned int period);

//==============================================================================
// Module Code
//==============================================================================

void enableStepperMotors(void)
{
    SM_E = 1;
}

void disableStepperMotors(void)
{
    SM_E = 0;
}

void stepMotor(enum Direction direction,
                  enum StepperSpeed speed,
                  unsigned int steps)
{
    unsigned int period = (speed == HIGH) ? HIGH_PERIOD : LOW_PERIOD;

    unsigned int i;
    for (i = 0; i < steps; i++)
    {
        advanceStep(&step1, direction == FORWARD ? INCREMENT : DECREMENT);

        SM1_A = STEP_A[step1];
        SM1_B = STEP_B[step1];

        delay(period);
    }
}

void testStepperMotors(enum StepperSpeed speed)
{
    unsigned int steps = 1000;

    enableStepperMotors();

    stepMotor(FORWARD, speed, steps);
    stepMotor(REVERSE, speed, steps);

    disableStepperMotors();
}

void advanceStep(unsigned int *step, enum StepDirection direction)
{
    if ((*step) == 0 && direction == DECREMENT)
    {
        (*step) = num_steps - 1;
    }
    else
    {
        (*step) += (direction == INCREMENT) ? 1 : -1;

        if ((*step) >= num_steps)
            (*step) = 0;
    }
}

void delay(unsigned int period)
{
//    STEPPER_TIMER = 0;
//    while (STEPPER_TIMER < period) {}
    TMR1 = 0;
    while (TMR1 < period) {}
}
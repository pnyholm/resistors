#include "StepperMotors.h"

//==============================================================================
// Private Module Constants
//==============================================================================

static const unsigned int num_steps = 4;

static const char STEP_A[] = {1, 1, 0, 0};
static const char STEP_B[] = {0, 1, 1, 0};

static const unsigned int LOW_PERIOD = 625; // 100 Hz
static const unsigned int HIGH_PERIOD = 142; // 440 Hz

//==============================================================================
// Private Module Variables
//==============================================================================

unsigned int step1 = 0;
unsigned int step2 = 0;

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

void stepTogether(enum TogetherDirections direction,
                  enum StepperSpeed speed,
                  unsigned int steps)
{
    unsigned int period = (speed == HIGH) ? HIGH_PERIOD : LOW_PERIOD;

    unsigned int i;
    for (i = 0; i < steps; i++)
    {
        advanceStep(&step1, direction == FORWARD ? INCREMENT : DECREMENT);
        advanceStep(&step2, direction == FORWARD ? INCREMENT : DECREMENT);

        SM1_A = STEP_A[step1];
        SM1_B = STEP_B[step1];

        SM2_A = STEP_A[step2];
        SM2_B = STEP_B[step2];

        delay(period);
    }
}

void stepOpposite(enum OppositeDirections direction,
                  enum StepperSpeed speed,
                  unsigned int steps)
{
    unsigned int period = (speed == HIGH) ? HIGH_PERIOD : LOW_PERIOD;

    unsigned int i;
    for (i = 0; i < steps; i++)
    {
        advanceStep(&step1, (direction == LEFT) ? INCREMENT : DECREMENT);
        advanceStep(&step2, (direction == LEFT) ? DECREMENT : INCREMENT);

        SM1_A = STEP_A[step1];
        SM1_B = STEP_B[step1];

        SM2_A = STEP_A[step2];
        SM2_B = STEP_B[step2];

        delay(period);
    }
}

void testStepperMotors(enum StepperSpeed speed)
{
    unsigned int steps = 1000;

    enableStepperMotors();

    stepTogether(FORWARD, speed, steps);
    stepTogether(REVERSE, speed, steps);

    stepOpposite(LEFT, speed, steps);
    stepOpposite(RIGHT, speed, steps);

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
    STEPPER_TIMER = 0;
    while (STEPPER_TIMER < period) {}
}
#include "RangeFinders.h"

//==============================================================================
// Define Global Variables
//==============================================================================

char RF_E = 0;

unsigned char RF_ECHO_FLAG = 0;
unsigned char RF_READ_FLAG = 0;
unsigned int RF_RANGE = 0;

//==============================================================================
// Private Module Constants
//==============================================================================

static const unsigned int TRIGGER_PERIOD = 10;

static const int TOLERANCE = 2;
// max possible value for 48in = (48 * 148)E-6 * 62500 = 7104

//==============================================================================
// Private Module Variables
//==============================================================================


//==============================================================================
// Private Module Function Prototypes
//==============================================================================

enum RangeFinderSelect { FRONT, BACK };
unsigned int getRange(enum RangeFinderSelect select);

//==============================================================================
// Module Code
//==============================================================================

void initRangeFinders(void)
{
    RF_E = 1;
}

void disableRangeFinders(void)
{
    RF_E = 0;
    RF1_E = 0;
    RF2_E = 0;
}

enum RangeFinderResult compareRanges(void)
{
    long int front = getRange(FRONT);
    long int back = getRange(BACK);

    long int difference = front - back;

    if (difference > TOLERANCE)
    {
        DEBUG1 = 1;
        DEBUG2 = 0;
        return MOVE_FORWARD;
    }
    else if (difference < -TOLERANCE)
    {
        DEBUG1 = 0;
        DEBUG2 = 1;
        return MOVE_BACKWARD;
    }
    else
    {
        DEBUG1 = 1;
        DEBUG2 = 1;
        return CENTERED;
    }
}

unsigned int getRange(enum RangeFinderSelect select)
{
    RF_READ_FLAG = 0;

    switch (select)
    {
    case FRONT:
        RF1_E = 1;
        RF2_E = 0;

        RF1_TRIG = 1;
        TIMER1 = 0;
        while (TIMER1 < TRIGGER_PERIOD) {}
        RF1_TRIG = 0;

        break;


    case BACK:
        RF1_E = 0;
        RF2_E = 1;
        
        RF2_TRIG = 1;
        TIMER1 = 0;
        while (TIMER1 < TRIGGER_PERIOD) {}
        RF2_TRIG = 0;
        
        break;
    }

    while (!RF_READ_FLAG) {}

    TIMER1 = 0;
    while (TIMER1 < 10);

    return RF_RANGE;
}

void testRangeFinders(void)
{
    compareRanges();
}
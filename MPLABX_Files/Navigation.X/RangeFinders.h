/* 
 * File:   RangeFinders.h
 * Author: Daniel Koch
 *
 * Created on August 8, 2013, 11:55 PM
 */

#ifndef RANGEFINDERS_H
#define	RANGEFINDERS_H

#include <p24F04KA201.h>
#include "GlobalConfig.h"

//==============================================================================
// Public Module Data
//==============================================================================

extern char RF_E;

enum RangeFinderResult { MOVE_FORWARD, MOVE_BACKWARD, CENTERED };

extern unsigned char RF_ECHO_FLAG;
extern unsigned char RF_READ_FLAG;
extern unsigned int RF_RANGE;

//==============================================================================
// Public Module Functions
//==============================================================================

void initRangeFinders(void);
void disableRangeFinders(void);

enum RangeFinderResult compareRanges(void);

void testRangeFinders(void);

#endif	/* RANGEFINDERS_H */


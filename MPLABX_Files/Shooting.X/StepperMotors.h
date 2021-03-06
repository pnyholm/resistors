/* 
 * File:   StepperMotors.h
 * Author: Daniel Koch
 *
 * Created on August 8, 2013, 10:24 PM
 */

#ifndef STEPPERMOTORS_H
#define	STEPPERMOTORS_H

#include <p24F04KA201.h>
#include "GlobalConfig.h"

//==============================================================================
// Public Module Data
//==============================================================================

enum StepperSpeed { LOW, HIGH };
enum Direction { FORWARD, REVERSE };

//==============================================================================
// Public Module Functions
//==============================================================================

// turn on power to motors
void enableStepperMotors();

// turn off power to motors
void disableStepperMotors();

// step both motors in the same direction
void stepMotor(enum Direction direction,
                  enum StepperSpeed speed,
                  unsigned int steps);

// runs a simple test on the stepper motors
void testStepperMotors(enum StepperSpeed speed);

#endif	/* STEPPERMOTORS_H */


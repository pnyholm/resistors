/* 
 * File:   ColorDetection.h
 * Author: pnyholm
 *
 * Created on August 9, 2013, 6:57 AM
 */

#ifndef COLORDETECTION_H
#define	COLORDETECTION_H

#include <p24F04KA201.h>
#include <stdlib.h>
#include "GlobalConfig.h"

//==============================================================================
// Public Data
//==============================================================================
enum BallColor { PURPLE, PINK, WHITE };

//==============================================================================
// Public Functions
//==============================================================================
void configureADC(void);
enum BallColor detectColor(void);

#endif	/* COLORDETECTION_H */


/*
 * File:   SPIconfiguration.h
 * Author: pnyholm
 *
 * Created on July 19, 2013, 4:34 PM
 */

#ifndef SPICONFIGURATION_H
#define	SPICONFIGURATION_H

#include <p24F04KA201.h>
#include "GlobalConfig.h"

void InitSPISlave(void);
void configureSlaveSPI(void);
unsigned short ReadSPI(void);

#endif	/* SPICONFIGURATION_H */

#ifndef _GAMECORE_H_
#define _GAMECORE_H_

#include <krandomsequence.h>

#include "sector.h"

//**********************************************************
// Core Logic routines
//**********************************************************

class CoreLogic
{
public:
    CoreLogic();

    Coordinate  generatePlanetCoordinates();
    double      generateKillPercentage();
    int         generatePlanetProduction();

    double      roll();

private:
    KRandomSequence  random;
};

#endif // _GAMECORE_H_



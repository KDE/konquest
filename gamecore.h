#ifndef _GAMECORE_H_
#define _GAMECORE_H_

#include <krandomsequence.h>

#include <QPoint>
#include "planet.h"

// Board Size Constants
#define BOARD_ROWS 16
#define BOARD_COLS 16

// Maximum Number of Players
#define MAX_PLAYERS 10


// -------------------------------------------------------------------------
// Typedefs
// -------------------------------------------------------------------------

typedef QPoint Coordinate;  // Gotta start using this instead of int x,y crap


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
    double      generateMorale();

    double      distance( Planet *p1, Planet *p2 );

    double      roll();

private:
    KRandomSequence  random;
};

#endif // _GAMECORE_H_



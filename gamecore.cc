#include "gamecore.h"

#include <math.h>

#include <config.h>
#include "planet.h"
#include "sector.h"

//*******************************************************************
// Game Core Logic
//*******************************************************************

CoreLogic::CoreLogic()
{
    random.setSeed(0);
}

Coordinate
CoreLogic::generatePlanetCoordinates()
{
    // 0 - 15
    return Coordinate(random.getLong(16), random.getLong(16));
}

double
CoreLogic::generateKillPercentage()
{
    // 0.30 - 0.90
    return 0.30 + random.getDouble()*0.60;
}

int
CoreLogic::generatePlanetProduction()
{
    // 5 - 15
    return 5 + random.getLong(10);
}


double
CoreLogic::roll()
{
    // 0.00 - 1.00
    return random.getDouble();
}


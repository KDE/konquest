#include "gamecore.h"
#include "sector.h"

//*******************************************************************
// Game Core Logic
//*******************************************************************

CoreLogic::CoreLogic()
{
    random.setSeed(0);
}

Coordinate
CoreLogic::generatePlanetCoordinates(int x, int y)
{
    return Coordinate(random.getLong(x), random.getLong(y));
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


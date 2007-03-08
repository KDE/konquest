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
CoreLogic::generatePlanetCoordinates(int rows, int cols)
{
    return Coordinate(random.getLong(rows), random.getLong(cols));
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


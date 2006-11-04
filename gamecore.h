#ifndef _GAMECORE_H_
#define _GAMECORE_H_

#include <krandomsequence.h>

#include <QObject>
#include <QPoint>
#include <QString>
#include <QColor>
#include <q3ptrlist.h>

// Board Size Constants
#define BOARD_ROWS 16
#define BOARD_COLS 16

// Maximum Number of Players
#define MAX_PLAYERS 10


//**********************************************************
// Forward declarations for classes in this file
//**********************************************************
class Player;
class Planet;
class Map;
class Fleet;
class AttackFleet;
class DefenseFleet;


//---------------------------------------------------------------------------------
// Typedefs
//---------------------------------------------------------------------------------
typedef QPoint Coordinate;  // Gotta start using this instead of int x,y crap


//**********************************************************
// Core Logic routines
//**********************************************************

class CoreLogic
{
public:
    CoreLogic();

    Coordinate generatePlanetCoordinates();
    double generateKillPercentage();
    int generatePlanetProduction();
    double generateMorale();

    double distance( Planet *p1, Planet *p2 );

    double roll();

private:
    KRandomSequence random;
};

//***************************************************************
// class Sector
//***************************************************************

class Sector : public QObject
{
    Q_OBJECT

public:

    // constructors
    Sector();
    Sector( Map *parentMap, Coordinate c );
    Sector( const Sector & );

    // assignment operator (makes initialization easy)
    Sector &operator=( const Sector & );

    bool hasPlanet() const;
    void setPlanet( Planet *newPlanet );
    Planet *getPlanet();
    void removePlanet();

    void select();

    Coordinate getCoord();

signals:
    void update();
    void selected();

protected slots:
    void childPlanetUpdate( );

    
protected:
    Planet *planet;  // a sector has 0 or 1 planets
    Map *parentMap;
    Coordinate c;

};
 
//*****************************************************************
// class Map
//*****************************************************************

class Map : public QObject
{
    Q_OBJECT

public:
    Map();
    virtual ~Map();

    const int getRows() const;
    const int getColumns() const;

    void populateMap( Q3PtrList<Player> &players, Player *neutral,
                      int numNeutralPlanets, Q3PtrList<Planet> &thePlanets );
    void clearMap();
    
    bool selectedSector( Coordinate &c ) const;
    void setSelectedSector( Coordinate c );
    void setSelectedSector( const Planet & );
    void setSelectedSector();

    Sector &getSector( Coordinate c );

protected slots:
    void childSectorUpdate();

signals:
    void update();

protected:
    void Freeze();
    void Thaw();
    bool freezeUpdates;

private:
    Sector &findRandomFreeSector();
    
protected:
    Sector grid[BOARD_ROWS][BOARD_COLS];  // a map is a 2-D array of sectors;
    const int rows; // size of grid in sectors
    const int columns;

    // This is used to implement a selected sector,
    // one who's boarder flashes.
    bool hasSelectedSector;
    Coordinate  sel;
};

#endif // _GAMECORE_H_



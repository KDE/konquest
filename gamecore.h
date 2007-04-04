#ifndef _GAMECORE_H_
#define _GAMECORE_H_

#include <krandomsequence.h>

#include <qobject.h>
#include <qstring.h>
#include <qcolor.h>
#include <qptrlist.h>

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
class Sector;
class Map;
class Fleet;


//**********************************************************
// Core Logic routines
//**********************************************************

class CoreLogic
{
public:
    CoreLogic();

    void generatePlanetCoordinates( int &x, int &y );
    double generateKillPercentage();
    int generatePlanetProduction();
    double generateMorale();

    double distance( Planet *p1, Planet *p2 );

    double roll();

private:
    KRandomSequence random;
    static bool class_init;
};


//**********************************************************
// class Fleet
// \--- class AttackFleet
//  \--- class DefenseFleet
//**********************************************************

class Fleet : public QObject
{
        
public:

    Fleet( int initialShipCount );
    virtual ~Fleet() {}

    int getShipCount();
    void removeShips( int lostShips );

protected:
    int shipCount;
};

class AttackFleet : public Fleet
{

public:
    AttackFleet( Planet *source, Planet *dest, int initialCount, double arrivalTurn  );

    Player *owner;
    Planet *destination;
    double arrivalTurn;
    double killPercentage;

};


class DefenseFleet : public Fleet
{
        
public:
    DefenseFleet( Planet *newHome, int initialCount  );

    void absorb( AttackFleet *fleet );
    void become( AttackFleet *fleet );

    void addShips( int newShips );
    
    AttackFleet *spawnAttackFleet( Planet *destination, int shipCount, double arrivalTurn );

    Planet *home;

};


//**************************************************************
// class Player
//**************************************************************

class Player : public QObject
{

public:
    Player( QString newName, QColor color, int number, bool isAi );
    virtual ~Player();

    enum { NEUTRAL_PLAYER_NUMBER = -1 };
    
public:
    QString &getName();
    QString getColoredName();
    QColor &getColor();
    bool isNeutral();
    QPtrList<AttackFleet> &getAttackList();

    // factory functions
    static Player *createPlayer( QString newName, QColor newColor, int playerNum, bool isAi  );
    static Player *createNeutralPlayer();

    bool NewAttack( Planet *sourcePlanet, Planet *destPlanet, int shipCount, int departureTurn );

    bool operator==( const Player &otherPlayer ) const;

    bool isInPlay();
    void setInPlay( bool );
    
private:
    QString name;
    QColor  color;
    int playerNum;
    bool inPlay;
    bool aiPlayer;

    QPtrList<AttackFleet> attackList;

    // statistics counters
    int shipsBuilt;
    int planetsConquered;
    int fleetsLaunched;
    int enemyFleetsDestroyed;
    int enemyShipsDestroyed;

public:
    void statShipsBuilt( int );
    void statPlanetsConquered( int );
    void statFleetsLaunched( int );
    void statEnemyFleetsDestroyed( int );
    void statEnemyShipsDestroyed( int );

    int getShipsBuilt() { return shipsBuilt; }
    int getPlanetsConquered() { return  planetsConquered; }
    int getFleetsLaunched() { return  fleetsLaunched; }
    int getEnemyFleetsDestroyed() { return  enemyFleetsDestroyed; }
    int getEnemyShipsDestroyed() { return  enemyShipsDestroyed; }
    bool isAiPlayer();

};


//**************************************************************
// class Planet
//**************************************************************

class Planet : public QObject
{
    Q_OBJECT

private:

    Planet( QString planetName, Sector &newParentSector,
            Player *initialOwner, int newProd,
            double newKillP, double newMorale );

public:
    virtual ~Planet();

    static Planet *createPlayerPlanet( Sector &parentSector,
                                       Player *initialOwner, QString planetName );
    static Planet *createNeutralPlanet( Sector &parentSector,
                                        Player *initialOwner, QString planetName );

    Sector &getSector() const;
    Player *getPlayer() const;
    const QString &getName() const;
    DefenseFleet &getFleet();

    double getKillPercentage();
    void setKillPercentage( double newValue );
    double getMorale();
    void setMorale( double );
    int getProduction();
    void setProduction( int );

    void select();
    void conquer(  AttackFleet *conqueringFleet );
    void coup( Player *luckyPlayer );
    void turn();

signals:
    void update();
    void selected();

private:
    QString name;
    Player *owner;
    Sector &parentSector;
    DefenseFleet homeFleet;

    double killPercentage;
    double morale;
    int productionRate;
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
    Sector( Map *parentMap, int xpos, int ypos );
    Sector( const Sector & );

    // assignment operator (makes initialization easy)
    Sector &operator=( const Sector & );

    bool hasPlanet() const;
    void setPlanet( Planet *newPlanet );
    Planet *getPlanet();
    void removePlanet();

    void select();
    
    int getRow();
    int getColumn();

signals:
    void update();
    void selected();

protected slots:
    void childPlanetUpdate( );

    
protected:
    Planet *planet;  // a sector has 0 or 1 planets
    Map *parentMap;
    int x, y;

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

    void populateMap( QPtrList<Player> &players, Player *neutral,
                      int numNeutralPlanets, QPtrList<Planet> &thePlanets );
    void clearMap();
    
    bool selectedSector( int &x, int &y ) const;
    void setSelectedSector( int x, int y );
    void setSelectedSector( const Planet & );
    void setSelectedSector();

    Sector &getSector( int x, int y );

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
    int  sel_x, sel_y;
};

//---------------------------------------------------------------------------------
// Typedefs
//---------------------------------------------------------------------------------
typedef QPoint Coordinate;  // Gotta start using this instead of int x,y crap
typedef QPtrList<AttackFleet> AttackFleetList;
typedef QPtrListIterator<AttackFleet> AttackFleetListIterator;
typedef QPtrList<Player> PlayerList;
typedef QPtrList<Planet> PlanetList;
typedef QPtrListIterator<Player> PlayerListIterator;
typedef QPtrListIterator<Planet> PlanetListIterator;

#endif // _GAMECORE_H_



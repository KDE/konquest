#ifndef _GAMECORE_H_
#define _GAMECORE_H_

#include <stdlib.h>

#include <qobject.h>
#include <qstring.h>
#include <qcolor.h>
#include <qlist.h>

// Board Size Constants
#define BOARD_ROWS 16
#define BOARD_COLS 16

// Maximum Number of Players
#define MAX_PLAYERS 5


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
    double generateKillPercentage( void );
    int generatePlanetProduction( void );
    double generateMorale( void );

    double distance( Planet *p1, Planet *p2 );

    double roll( void );

private:
    static bool class_init;
};


//**********************************************************
// class Fleet
// \--- class AttackFleet
//  \--- class DefenseFleet
//**********************************************************

class Fleet : public QObject
{
    Q_OBJECT
        
public:

    Fleet( int initialShipCount );
    virtual ~Fleet() {};

    int getShipCount();
    void removeShips( int lostShips );

protected:
    int shipCount;
};

class AttackFleet : public Fleet
{
    Q_OBJECT
public:
    AttackFleet( Planet *source, Planet *dest, int initialCount, double arrivalTurn  );

    Player *owner;
    Planet *destination;
    double arrivalTurn;
    double killPercentage;

};


class DefenseFleet : public Fleet
{
    Q_OBJECT
        
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
    Q_OBJECT

public:
    Player( QString newName, QColor color, int number  );
    virtual ~Player();

    enum { NEUTRAL_PLAYER_NUMBER = -1 };
    
public:
    QString &getName( void );
    QColor &getColor( void );
    bool isNeutral( void );
    QList<AttackFleet> &getAttackList( void );

    // factory functions
    static Player *createPlayer( QString newName, QColor newColor, int playerNum  );
    static Player *createNeutralPlayer( void );

    bool NewAttack( Planet *sourcePlanet, Planet *destPlanet, int shipCount, int departureTurn );

    bool operator==( const Player &otherPlayer ) const;

    bool isInPlay( void );
    void setInPlay( bool );
    
private:
    QString name;
    QColor  color;
    int playerNum;
    bool inPlay;

    QList<AttackFleet> attackList;

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

    int getShipsBuilt( void ) { return shipsBuilt; };
    int getPlanetsConquered( void ) { return  planetsConquered; };
    int getFleetsLaunched( void ) { return  fleetsLaunched; };
    int getEnemyFleetsDestroyed( void ) { return  enemyFleetsDestroyed; };
    int getEnemyShipsDestroyed( void ) { return  enemyShipsDestroyed; };

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
                                       Player *initalOwner, QString planetName );
    static Planet *createNeutralPlanet( Sector &parentSector,
                                        Player *initialOwner, QString planetName );

    Sector &getSector( void ) const;
    Player *getPlayer( void ) const;
    const QString &getName( void ) const;
    DefenseFleet &getFleet( void );

    double getKillPercentage( void );
    void setKillPercentage( double newValue );
    double getMorale( void );
    void setMorale( double );
    int getProduction( void );
    void setProduction( int );

    void select( void );
    void conquer(  AttackFleet *conqueringFleet );
    void coup( Player *luckyPlayer );
    void turn( void );

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

    const int getRows( void ) const;
    const int getColumns( void ) const;

    void populateMap( QList<Player> &players, Player *neutral,
                      int numNeutralPlanets, QList<Planet> &thePlanets );
    void clearMap( void );
    
    bool selectedSector( int &x, int &y ) const;
    void setSelectedSector( int x, int y );
    void setSelectedSector( const Planet & );
    void setSelectedSector( void );

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
    Sector &findRandomFreeSector( void );
    
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
typedef QList<AttackFleet> AttackFleetList;
typedef QListIterator<AttackFleet> AttackFleetListIterator;
typedef QList<Player> PlayerList;
typedef QList<Planet> PlanetList;
typedef QListIterator<Player> PlayerListIterator;
typedef QListIterator<Planet> PlanetListIterator;

#endif // _GAMECORE_H_



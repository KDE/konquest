#ifndef _GAMEBOARD_H_
#define _GAMEBOARD_H_

#include <qwidget.h>

#include "planet_info.h"
#include "map_widget.h"

//************************************************************************
// forward declarations
//************************************************************************
class QSlider;
class QLabel;
class QListBox;
class QPushButton;
class QLineEdit;

enum GameState { NONE, SOURCE_PLANET, DEST_PLANET, SHIP_COUNT, RULER_SOURCE, RULER_DEST };

//************************************************************************
// GameBoard Widget
//************************************************************************
class GameBoard : public QWidget
{
    Q_OBJECT;

public:
    GameBoard( QWidget *parent );
    virtual ~GameBoard();

    bool isGameInProgress(void) { return gameInProgress; };

protected slots:
    void startNewGame();
    void shutdownGame();
    void planetSelected( Planet * );
    void newShipCount();
    void nextPlayer();

    //***************************************************************
    // Toolbar items
    //***************************************************************
    void measureDistance();
    void showScores();

signals:
    void newGameState( GameState newState );
    
    //***************************************************************
    // Event Handlers
    //***************************************************************
protected:
    void keyPressEvent( QKeyEvent * );

private:
    void turn( void );
    void nextTurn( void );

    void resolveShipsInFlight( void );
    void sendAttackFleet( void );
    void doFleetArrival( AttackFleet *arrivingFleet );
    void scanForSurvivors( void );

    void changeGameBoard( bool inPlay );
    void cleanupGame( void );

    //***************************************************************
    // Game State information
    //***************************************************************
    bool gameInProgress;
    GameState gameState;
    PlayerListIterator *currentPlayer;

    //***************************************************************
    // Display Widgets
    //***************************************************************
    ConquestMap *mapWidget;
    PlanetInfo *planetInfo;
    QLabel *gameMessage;
    QLabel *turnCounter;
    QPushButton *endTurn;
    QLineEdit *shipCountEdit;
    QLabel *splashScreen;


    //***************************************************************
    // Game objects
    //***************************************************************
    int turnNumber;
    int lastTurn;
    
    PlayerList players;
    PlanetList planets;
    Player *neutralPlayer;
    Map *map;

    bool haveSourcePlanet;
    Planet *sourcePlanet;

    bool haveDestPlanet;
    Planet *destPlanet;

    bool haveShipCount;
    int shipCount;

};

#endif

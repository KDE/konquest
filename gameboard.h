#ifndef _GAMEBOARD_H_
#define _GAMEBOARD_H_

#include <QWidget>
#include "planet.h"
#include "player.h"

//************************************************************************
// forward declarations
//************************************************************************

class QSlider;
class QLabel;
class QPushButton;
class QLineEdit;
class QTextEdit;

class Player;
class MapView;
class PlanetInfo;

class KLocalizedString;

enum GameState { 
    NONE, 
    SOURCE_PLANET, 
    DEST_PLANET, 
    SHIP_COUNT, 
    RULER_SOURCE, 
    RULER_DEST, 
    AI_PLAYER
};

//************************************************************************
// GameBoard Widget
//************************************************************************
class GameBoard : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoard( QWidget *parent );
    virtual ~GameBoard();

    bool isGameInProgress(void) const { return gameInProgress; };

    int turnNumber() const { return m_turnNumber; };
    int lastTurn() const { return m_lastTurn; };
//    virtual QSize sizeHint() const;

protected slots:
    void  startNewGame();
    void  shutdownGame();
    void  planetSelected( Planet * );
    void  newShipCount();
    void  nextPlayer();

    //***************************************************************
    // Toolbar items
    //***************************************************************
    void  measureDistance();
    void  showScores();
    void  showFleets();

signals:
    void  newGameState( GameState newState );

    //***************************************************************
    // Event Handlers
    //***************************************************************
protected:
    virtual void  keyPressEvent( QKeyEvent * );

private:
    void     turn();
    void     nextTurn();
    void     gameOver();

    void     resolveShipsInFlight();
    void     sendAttackFleet( Planet *source, Planet *dest, int ships );
    void     doFleetArrival( AttackFleet *arrivingFleet );
    void     scanForSurvivors();

    void     gameMsg(const KLocalizedString &msg, Player *player = 0,
		     Planet *planet = 0, Player *planetPlayer = 0);

    void     changeGameBoard( bool inPlay );
    void     cleanupGame();
    Player  *findWinner();
    
    //***************************************************************
    // Game State information
    //***************************************************************

    bool                       gameInProgress;
    GameState                  gameState;
    QList<Player *>::Iterator  currentPlayer;
    int                        m_turnNumber;
    int                        m_lastTurn;


    //***************************************************************
    // Display Widgets
    //***************************************************************

    MapView      *mapWidget;
    QLabel       *gameMessage;
    QPushButton  *endTurn;
    QLineEdit    *shipCountEdit;
    QLabel       *splashScreen;
    QTextEdit    *msgWidget;


    //***************************************************************
    // Game objects
    //***************************************************************

    QList<Player *>  players;
    QList<Planet *>  planets;
    Player          *neutralPlayer;
    Map             *map;

    // States in the user interaction
    bool             haveSourcePlanet;
    Planet          *sourcePlanet;

    bool             haveDestPlanet;
    Planet          *destPlanet;

    bool             haveShipCount;
    int              shipCount;

};

#endif

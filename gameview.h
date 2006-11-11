#ifndef _GAMEVIEW_H_
#define _GAMEVIEW_H_

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
class GameLogic;

class KLocalizedString;

struct GameMessage {
    QString text;
    Player *sender;
    Player *receiver;
};

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
// GameView Widget
//************************************************************************
class GameView : public QWidget
{
    Q_OBJECT

public:
    explicit GameView( QWidget *parent, GameLogic *gameLogic );
    virtual ~GameView();

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

    void     gameMsg(const KLocalizedString &msg, Player *player = 0,
		     Planet *planet = 0, Player *planetPlayer = 0);

    void     changeGameView( bool inPlay );
    void     cleanupGame();
    

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

    GameLogic       *m_gameLogic;

    Player          *neutralPlayer;  // FIXME: Have here or in gamelogic?
    bool             queueMessages;
    QList<GameMessage> messageQueue;

    // States in the user interaction
    bool             haveSourcePlanet;
    Planet          *sourcePlanet;

    bool             haveDestPlanet;
    Planet          *destPlanet;

    bool             haveShipCount;
    int              shipCount;

    //***************************************************************
    // Game State information
    //***************************************************************

    bool                       gameInProgress;
    GameState                  gameState;
};

#endif

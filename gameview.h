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

class KLocalizedString;

class Player;
class MapView;
class MapScene;
class PlanetInfo;
class GameLogic;


struct GameMessage {
    QString  text;
    Player  *sender;
    Player  *receiver;
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
    explicit  GameView( QWidget *parent, GameLogic *gameLogic );
    virtual  ~GameView();

//    virtual QSize sizeHint() const;

    bool  isGameInProgress() const { return m_gameInProgress; }

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

public slots:
    void  gameMsg(const KLocalizedString &msg, Player *player = 0,
		  Planet *planet = 0, Player *planetPlayer = 0);
    void  beginTurn();
    void  endTurn();

signals:
    void  newGameState( GameState newState );

    //***************************************************************
    // Event Handlers
    //***************************************************************
protected:
    virtual void  keyPressEvent( QKeyEvent * );

private:
    void  turn();
    void  gameOver();

    void  sendAttackFleet( Planet *source, Planet *dest, int ships );

    void  changeGameView( bool inPlay );
    void  cleanupGame();
    

    //***************************************************************
    // Display Widgets
    //***************************************************************

    MapView      *m_mapWidget;
    MapScene     *m_mapScene;
    QLabel       *m_gameMessage;
    QPushButton  *m_endTurnBtn;
    QLineEdit    *m_shipCountEdit;
    QLabel       *m_splashScreen;
    QTextEdit    *m_msgWidget;

    //***************************************************************
    // Game objects
    //***************************************************************

    GameLogic          *m_gameLogic;

    Player             *m_neutralPlayer;  // FIXME: Have here or in gamelogic?
    bool                m_queueMessages;
    QList<GameMessage>  m_messageQueue;

    // States in the user interaction
    // FIXME: Break out into another file?
    bool                haveSourcePlanet;
    Planet             *sourcePlanet;

    bool                haveDestPlanet;
    Planet             *destPlanet;

    bool                haveShipCount;
    int                 shipCount;

    //***************************************************************
    // Game State information
    //***************************************************************

    bool                m_gameInProgress;
    GameState           m_gameState;
};


#endif

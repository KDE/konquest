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
class QTextEdit;

enum GameState { NONE, SOURCE_PLANET, DEST_PLANET, SHIP_COUNT, RULER_SOURCE, RULER_DEST, AI_PLAYER };

//************************************************************************
// GameBoard Widget
//************************************************************************
class GameBoard : public QWidget
{
    Q_OBJECT

public:
    GameBoard( QWidget *parent );
    virtual ~GameBoard();

    bool isGameInProgress(void) const { return gameInProgress; }

//    virtual QSize sizeHint() const;

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
    void showFleets();

signals:
    void newGameState( GameState newState );

    //***************************************************************
    // Event Handlers
    //***************************************************************
protected:
    virtual void keyPressEvent( QKeyEvent * );

private:
    void turn();
    void nextTurn();
    void gameOver();

    void resolveShipsInFlight();
    void sendAttackFleet( Planet *source, Planet *dest, int ships );
    void doFleetArrival( AttackFleet *arrivingFleet );
    void scanForSurvivors();

    void gameMsg(const QString &msg, Player *player = 0, Planet *planet = 0, Player *planetPlayer = 0);

    void changeGameBoard( bool inPlay );
    void cleanupGame();
    Player *findWinner();

    QString playerString(Player *player = 0);
    
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
    QTextEdit *msgWidget;


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

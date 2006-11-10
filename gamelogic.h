#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include <QWidget>
#include "planet.h"
#include "player.h"

//************************************************************************
// forward declarations
//************************************************************************


class Player;

class KLocalizedString;

struct GameMessage {
    QString text;
    Player *sender;
    Player *receiver;
};


//************************************************************************
// GameLogic Widget
//************************************************************************
class GameLogic : public QObject
{
    Q_OBJECT

public:
    explicit GameLogic( QObject *parent );
    virtual ~GameLogic();

    bool isGameInProgress(void) const { return gameInProgress; };

    int turnNumber() const { return m_turnNumber; };
    int lastTurn() const { return m_lastTurn; };

protected slots:
    void  startNewGame();
    void  shutdownGame();
    void  nextPlayer();


signals:
    void  gameOver(Player *winner);
    void     gameMsg(const KLocalizedString &msg, Player *player = 0,
		     Planet *planet = 0, Player *planetPlayer = 0);
private:
    //void     turn();
    void     nextTurn();
    void     gameOver();

    void     resolveShipsInFlight();
    void     doFleetArrival( AttackFleet *arrivingFleet );
    void     scanForSurvivors();

    void     cleanupGame();
    Player  *findWinner();
    
    //***************************************************************
    // Game State information
    //***************************************************************

    bool                       gameInProgress;
    QList<Player *>::Iterator  currentPlayer;
    int                        m_turnNumber;
    int                        m_lastTurn;


    //***************************************************************
    // Game objects
    //***************************************************************

    QList<Player *>  players;
    QList<Planet *>  planets;
    Player          *neutralPlayer;
    Map             *map;
    bool             queueMessages;
    QList<GameMessage> messageQueue;
};


#endif  // _GAMELOGIC_H_

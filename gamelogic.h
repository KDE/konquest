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


//************************************************************************
// GameLogic Object
//************************************************************************
class GameLogic : public QObject
{
    Q_OBJECT

public:
    explicit GameLogic( QObject *parent );
    virtual ~GameLogic();

    bool isGameInProgress(void) const { return gameInProgress; }

    int   turnNumber() const { return m_turnNumber; }
    Map  *map()        const { return m_map; }
    QList<Player *>  *players() { return &m_players; }
    QList<Planet *>  *planets() { return &m_planets; }

    Player  *findWinner();
    void     resolveShipsInFlight();
    void     scanForSurvivors();

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

    void     doFleetArrival( AttackFleet *arrivingFleet );

    void     cleanupGame();
    
private:

    //***************************************************************
    // Game objects
    //***************************************************************

    QList<Player *>  m_players;
    QList<Planet *>  m_planets;
    Player          *neutralPlayer;
    Map             *m_map;

    //***************************************************************
    // Game State information
    //***************************************************************

    bool                       gameInProgress;
    QList<Player *>::Iterator  currentPlayer;
    int                        m_turnNumber;
};


#endif  // _GAMELOGIC_H_

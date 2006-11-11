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

    int   turnNumber() const { return m_turnNumber; }
    Map  *map()        const { return m_map; }
    QList<Player *>  *players() { return &m_players; }
    QList<Planet *>  *planets() { return &m_planets; }
    Player     *currentPlayer() const { return *m_currentPlayer; }

    Player  *findWinner();
    void     resolveShipsInFlight();
    void     scanForSurvivors();

    void     startNewGame();
    void     nextTurn();
    void     nextPlayer();

signals:
    void     gameOver(Player *winner);
    void     gameMsg(const KLocalizedString &msg, Player *player = 0,
		     Planet *planet = 0, Player *planetPlayer = 0);
private:

    void     doFleetArrival( AttackFleet *arrivingFleet );
    void     cleanupGame();
    
private:

    //***************************************************************
    // Game objects
    //***************************************************************

    int              m_turnNumber;
    QList<Player *>  m_players;
    QList<Planet *>  m_planets;
    Player          *neutralPlayer;
    Map             *m_map;
    QList<Player *>::Iterator  m_currentPlayer;
};


#endif  // _GAMELOGIC_H_

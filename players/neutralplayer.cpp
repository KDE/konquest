#include "neutralplayer.h"
#include "../game.h"
#include "../planet.h"
#include <QColor>
#include <QDebug>
#include <QApplication>

NeutralPlayer::NeutralPlayer(Game *game) :
    Player(game, QString(), Qt::gray)
{
}

bool NeutralPlayer::isNeutral()
{
    return true;
}

bool NeutralPlayer::isDead()
{
    return false;
}

void NeutralPlayer::play()
{
    qDebug() << "NeutralPlayer::play";
    // Go on each attack...
    foreach (Player *player, m_game->players()) {
        foreach (AttackFleet *fleet, player->attackList()) {
            if( m_game->turnCounter() == fleet->arrivalTurn ) {
                if (m_game->doFleetArrival(fleet)) {
                    player->attackDone(fleet);
                    fleet->deleteLater();
                }
            }
        }
    }

    // Go over each planet, adding its ships
    foreach (Planet *planet, m_game->map()->planets())
    {
        qDebug() << "Turn for planet " << planet->name();
        planet->turn(m_game->options());
    }

    Player *winner = m_game->findWinner();
    if (winner) {
        // GAME OVER !!
    }

    // After all that mess... It's done
    emit(donePlaying());
}

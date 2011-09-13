/*
    Copyright Pierre Ducroquet <pinaraf@pinaraf.info>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "computerplayer.h"
#include "../game.h"
#include "../planet.h"
#include <QDebug>
#include <cmath>
#include <values.h>

ComputerPlayer::ComputerPlayer(Game *game,
                               const QString &newName,
                               const QColor &color,
                               AiLevel level)
    : Player(game, newName, color),
    m_level(level)
{

}

void ComputerPlayer::play()
{
    qDebug() << "computer playing";
    int      ships = 0, minimumShips =10, shipCountFactor = 2;
    Planet  *target = 0;

    switch (m_level)
    {
    case Weak:
       minimumShips = 20;
       shipCountFactor = 2;
       break;
    case Normal: // Offensive
       minimumShips = 10;
       shipCountFactor = 2;
       break;
    case Hard: // Defensive
       minimumShips = 30;
       shipCountFactor = 3;
       break;
    }

    foreach (Planet *home, m_game->planets()) {
        if (home->player() == this) {
            bool  hasAttack = false;
            ships = (int)floor(home->ships() * 0.7 );

            if (ships >= minimumShips) {
                double  minDistance = 100;

                foreach (Planet *attack, m_game->planets()) {
                    if (attack->player() == this)
                        continue;

                    bool    skip = false;
                    double  dist = m_game->map()->distance( home, attack );

                    if (dist < minDistance && attack->ships() < ships ) {
                        foreach (AttackFleet *curFleet, attackList()) {
                            if (curFleet->destination == attack) {
                                skip = true;
                            }
                        }
                        if (skip)
                            continue;

                        target      = attack;
                        hasAttack   = true;
                        minDistance = dist;
                    }
                }

                if (hasAttack) {
                    m_game->attack( home, target, ships );
                } else {
                    minDistance = MAXDOUBLE;
                    int shipsToSend = 0;
                    bool hasDestination = false;

                    foreach (Planet *attack, m_game->planets()) {
                        bool    skip = false;
                        double  dist = m_game->map()->distance( home, attack );
                        int     homeships = (int)floor(home->ships() * 0.5 );

                        if (dist < minDistance
                            && attack->player() == this
                            && attack->ships() < homeships ) {
                            foreach (AttackFleet *curFleet, attackList()) {
                                if (curFleet->destination == attack) {
                                    skip = true;
                                }
                            }
                            if (skip)
                                continue;

                            shipsToSend = (int)floor( double(home->ships()
                                                             - attack->ships()) / shipCountFactor);

                            target         = attack;
                            hasDestination = true;
                            minDistance    = dist;
                        }
                    }
                    if (hasDestination) {
                        m_game->attack( home, target, shipsToSend );
                    }
                }
            }
        }
    }
    emit(donePlaying());
}

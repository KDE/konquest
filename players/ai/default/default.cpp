/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "default.h"
#include "../../../game.h"
#include "../../../planet.h"

#include <QDebug>
#include <cmath>
#include <cfloat>

AiDefault::AiDefault(Game *game,
                               const QString &newName,
                               const QColor &color,
                               AiLevel level)
    : ComputerPlayer(game, newName, color),
    m_level(level)
{

}


void AiDefault::play()
{
    //qDebug() << "computer playing";
    int      ships = 0, minimumShips =10, shipCountFactor = 2;
    Planet  *target = nullptr;

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

    for (Planet *home : m_game->planets()) {
        if (home->player() == this) {
            bool  hasAttack = false;
            ships = (int)floor(home->ships() * 0.7 );

            if (ships >= minimumShips) {
                double  minDistance = 100;

                for (Planet *attack : m_game->planets()) {
                    if (attack->player() == this)
                        continue;

                    bool    skip = false;
                    double  dist = m_game->map()->distance( home, attack );

                    if (dist < minDistance && attack->ships() < ships ) {
                        for (AttackFleet *curFleet : attackList()) {
                            if (curFleet->destination == attack) {
                                skip = true;
                                break;
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
                    minDistance = DBL_MAX;
                    int shipsToSend = 0;
                    bool hasDestination = false;

                    for (Planet *attack : m_game->planets()) {
                        bool    skip = false;
                        double  dist = m_game->map()->distance( home, attack );
                        int     homeships = (int)floor(home->ships() * 0.5 );

                        if (dist < minDistance
                            && attack->player() == this
                            && attack->ships() < homeships ) {
                            for (AttackFleet *curFleet : attackList()) {
                                if (curFleet->destination == attack) {
                                    skip = true;
                                    break;
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
    Q_EMIT donePlaying();
}

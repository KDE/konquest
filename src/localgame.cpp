/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "localgame.h"
#include <QDebug>
#include <QApplication>

LocalGame::LocalGame(QObject *parent) :
    Game(parent)
{
}

void LocalGame::start()
{
    if (!m_gameMachine.isRunning()) {
        buildMachine();
        //qDebug() << "Starting machine";
        m_gameMachine.start();
        qApp->processEvents();  // Really important : ignoring this will not apply the change soon enough
        //qDebug() << "Machine state" << m_gameMachine.isRunning();
    }
}

void LocalGame::stop()
{
    if (m_gameMachine.isRunning()) {
        m_gameMachine.stop();
        qApp->processEvents();  // Really important : ignoring this will not apply the change soon enough
        //qDebug() << "Machine state" << m_gameMachine.isRunning();
    }
}

void LocalGame::addPlayer(Player *player)
{
    player->setParent(&m_gameMachine);
    if (!m_players.contains(player))
        m_players << player;
}

void LocalGame::buildMachine()
{
    //qDebug() << "Building machine";
    if (m_gameMachine.isRunning())
        return;

    // Player is a subclass of QState and the constructor of Player already adds
    // the new Player object to m_gameMachine by passing it to the superclass
    // constructor QState(QState *parent = 0).
    // Accordingly, we can instantly go ahead with configuring the other
    // parts of the machine.

    m_gameMachine.setInitialState(m_neutral);

    connect(m_neutral, &NeutralPlayer::donePlaying, this, &LocalGame::playerIsDone);
    m_neutral->addTransition(m_neutral, &NeutralPlayer::donePlaying, m_players[0]);

    // Now add transitions
    for (int i = 0 ; i < m_players.count() ; i++)
    {
        Player *player = m_players[i];
        Player *nextPlayer;
        if (i+1 >= m_players.count())
            nextPlayer = m_neutral;
        else
            nextPlayer = m_players[i + 1];

        //qDebug() << "Adding transition from "
                 //<< player->name() << " to " << nextPlayer->name();
        player->addTransition(player, &Player::donePlaying, nextPlayer);
        connect(player, &Player::donePlaying, this, &LocalGame::playerIsDone);
    }
}

void LocalGame::playerIsDone()
{
    //qDebug() << "It seems a player is done :" << currentPlayer()->name();
}

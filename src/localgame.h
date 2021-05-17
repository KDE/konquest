/*
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@pinaraf.info>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LOCALGAME_H
#define LOCALGAME_H

#include "game.h"
#include "players/player.h"

class LocalGame : public Game
{
public:
    explicit LocalGame(QObject *parent = nullptr);

    void start() override;
    void stop() override;

public:
    void addPlayer(Player *player);
    void playerIsDone();

protected:
    void buildMachine() override;
};

#endif // LOCALGAME_H

/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_NEWGAMEDLG_H
#define KONQUEST_NEWGAMEDLG_H


#include <QDialog>

#include "planet.h"
#include "minimapview.h"
#include "ui_newGameDialog.h"
#include "../players/player.h"
#include "../players/player_gui.h"


// Maximum Number of Players
#define MAX_PLAYERS 10


class NewGameDlgUI : public QWidget, public Ui::newGameDialog
{
public:
    explicit NewGameDlgUI( QWidget *parent ) : QWidget( parent ) {
	setupUi( this );
    }
        ~NewGameDlgUI() {}
};


/*************************************************************************
 New Game Dialog
 ************************************************************************/


class NewGameDlg : public QDialog
{
public:
    NewGameDlg( QWidget *parent, Game *game);
    ~NewGameDlg();

    void  save();

protected:
    void  slotNewMap();
    void  slotUpdateNeutrals(int);
    void  slotAddPlayer(int);
    void  slotRemovePlayer();

    void  slotUpdateSelection(const Coordinate &coord);
    void  slotNewOwner(int);
    void  slotNewKillPercentage(double);
    void  slotNewProduction(int);

public:
    void  updateButtonOk();

private:
    void  updateOwnerCB();
    void  init();

    QList<PlayerGui *> m_selectablePlayer;
    QMenu            *m_playerTypeChooser;
    QPushButton      *okButton;

    Game             *m_game;
    Player           *m_neutral;

    NewGameDlgUI     *m_w;
};

#endif // KONQUEST_NEWGAMEDLG_H

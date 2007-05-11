/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>

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
#ifndef _NEWGAMEDLG_H_
#define _NEWGAMEDLG_H_


#include <kdialog.h>

#include "planet.h"
#include "minimapview.h"
#include "ui_newGameDialog.h"
#include "player.h"


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


class NewGameDlg : public KDialog
{
    Q_OBJECT

public:
    NewGameDlg( QWidget *parent, Map *map, QList<Player *> *playerList,
                Player *neutralPlayer, QList<Planet *> *planetList );
    ~NewGameDlg() {}

    void  save();

protected slots:
    void  slotNewMap();
    void  slotAddPlayer();
    void  slotRemovePlayer();
    void  slotOk();

private:
    void  init();

private:
    Map              *m_map;
    QList<Player *>  *m_players;
    QList<Planet *>  *m_planets;
    Player           *m_neutral;

    NewGameDlgUI     *m_w;
};

#endif

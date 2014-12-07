/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

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

#ifndef KONQUEST_MAINWIN_H
#define KONQUEST_MAINWIN_H

#include <QAction>
#include <kxmlguiwindow.h>

#include "game.h"
#include "gameview.h"

class QAction;
class QDockWidget;


class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    QSize sizeHint() const;

private:
    void setupActions();
    void setupGameView();
    void setupGUI();

private slots:
    void guiStateChange( GUIState );
    void startNewGame();
    void updateMessagesActionSlot();
    void updateStandingsActionSlot();

private:
    // Widgets
    Game       *m_game;
    GameView   *m_gameView;
    QLabel     *m_statusBarText;

    // Actions
    QAction *m_endTurnAction;
    QAction *m_messagesAction;
    QAction *m_standingsAction;

    QAction  *m_endGameAction;
    QAction  *m_measureAction;
    QAction  *m_fleetAction;

    QDockWidget *m_messagesDock;
    QDockWidget *m_standingsDock;
};

#endif // KONQUEST_MAINWIN_H

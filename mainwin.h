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
#ifndef KONQUEST_MAINWIN_H
#define KONQUEST_MAINWIN_H


#include <kxmlguiwindow.h>

#include "game.h"
#include "gameview.h"

class QAction;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void setupActions();
    void setupGameView();

private slots:
    void guiStateChange( GUIState );
    void startNewGame();

private:
    // Widgets
    Game       *m_game;
    GameView   *m_gameView;
    QLabel     *m_statusBarText;

    // Actions
    QAction  *m_endAction;
    QAction  *m_measureAction;
    QAction  *m_standingAction;
    QAction  *m_fleetAction;
};

#endif // KONQUEST_MAINWIN_H

/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_MAINWIN_H
#define KONQUEST_MAINWIN_H

#include <QAction>
#include <KXmlGuiWindow>

#include "game.h"
#include "gameview.h"

class QDockWidget;


class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    QSize sizeHint() const override;

private:
    void setupActions();
    void setupGameView();
    void setupGUI();

private Q_SLOTS:
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

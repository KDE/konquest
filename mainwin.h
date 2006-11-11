#ifndef _MAIN_WIN_H
#define _MAIN_WIN_H

#include <kmainwindow.h>

#if 0
#include "gameboard.h"
#else
#include "gamelogic.h"
#include "gameview.h"
#endif

class MainWindow : public KMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void setupActions();
    void setupGameBoard();

private slots:
    void gameStateChange( GameState );

private:
    // Widgets
#if 0
    GameBoard  *m_gameBoard;
#else
    GameLogic  *m_gameLogic;
    GameView   *m_gameView;
#endif
    QLabel     *m_statusBarText;

    // Actions
    KAction  *m_endAction;
    KAction  *m_measureAction;
    KAction  *m_standingAction;
    KAction  *m_fleetAction;


};

#endif


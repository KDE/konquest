#ifndef _MAIN_WIN_H
#define _MAIN_WIN_H

#include <kmainwindow.h>

#include "gameboard.h"

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
    GameBoard  *m_gameBoard;
    QLabel     *m_statusBarText;

    // Actions
    KAction  *m_endAction;
    KAction  *m_measureAction;
    KAction  *m_standingAction;
    KAction  *m_fleetAction;


};

#endif


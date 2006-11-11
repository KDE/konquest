#ifndef _MAIN_WIN_H
#define _MAIN_WIN_H


#include <kmainwindow.h>

#include "gamelogic.h"
#include "gameview.h"


class MainWindow : public KMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void setupActions();
    void setupGameView();

private slots:
    void gameStateChange( GameState );

private:
    // Widgets
    GameLogic  *m_gameLogic;
    GameView   *m_gameView;
    QLabel     *m_statusBarText;

    // Actions
    KAction  *m_endAction;
    KAction  *m_measureAction;
    KAction  *m_standingAction;
    KAction  *m_fleetAction;


};

#endif


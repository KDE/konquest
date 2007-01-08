#ifndef _MAIN_WIN_H
#define _MAIN_WIN_H


#include <kmainwindow.h>

#include "gamelogic.h"
#include "gameview.h"

class QAction;

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
    void guiStateChange( GUIState );

private:
    // Widgets
    GameLogic  *m_gameLogic;
    GameView   *m_gameView;
    QLabel     *m_statusBarText;

    // Actions
    QAction  *m_endAction;
    QAction  *m_measureAction;
    QAction  *m_standingAction;
    QAction  *m_fleetAction;


};

#endif


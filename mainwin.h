#ifndef _MAIN_WIN_H
#define _MAIN_WIN_H

#include <kmainwindow.h>

#include "gameboard.h"

class ConquestMap;
class PlanetStatusTable;

class MainWindow : public KMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void setupKAction();
    void setupGameBoard();

private:
    GameBoard *gameBoard;
    KAction *endAction, *measureAction, *standingAction, *fleetAction;

private slots:
    void gameStateChange( GameState );

};

#endif


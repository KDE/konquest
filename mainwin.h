#ifndef _MAIN_WIN_H
#define _MAIN_WIN_H

#include <kmainwindow.h>
#include <ktoolbar.h>
#include <kmenubar.h>
#include <qdialog.h>

#include "gameboard.h"

class ConquestMap;
class PlanetStatusTable;

class MainWindow : public KMainWindow
{
    Q_OBJECT

public:
    MainWindow( const char *name = 0 );
    ~MainWindow();


protected:
    void setupMenu();
    void setupToolBar();
    void setupGameBoard();

private:
    KMenuBar *menubar;
    KToolBar *toolbar;
    GameBoard *gameBoard;

private slots:
    void gameStateChange( GameState );
    
};

#endif


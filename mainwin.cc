#include <config.h>

#include <qpushbutton.h>

#include <kapplication.h>
#include <klocale.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kstdgameaction.h>

#include "version.h"
#include "gamecore.h"
#include "mainwin.h"
#include "mainwin.moc"
#include "gameboard.h"

// KonquestMainWindow


MainWindow::MainWindow()
{
    setCaption( i18n("Galactic Conquest") );

    setupGameBoard();
    setupKAction();
    setupGUI();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::setupKAction()
{
    KStdGameAction::gameNew( gameBoard, SLOT( startNewGame() ), actionCollection() );
    KStdGameAction::quit( this, SLOT( close() ), actionCollection() );
    endAction = KStdGameAction::end( gameBoard, SLOT( shutdownGame() ), actionCollection() );
    endAction->setEnabled(false);

    //AB: there is no icon for disabled - KToolBar::insertButton shows the
    //different state - KAction not :-(
    measureAction = new KAction( i18n("&Measure Distance"), "ruler", 0, gameBoard, SLOT( measureDistance() ), actionCollection(), "game_measure" );
    measureAction->setEnabled(false);
    standingAction = new KAction( i18n("&Show Standings"), "help", 0, gameBoard, SLOT( showScores() ), actionCollection(), "game_scores" );
    standingAction->setEnabled(false);
    fleetAction = new KAction( i18n("&Fleet Overview"), "launch", 0, gameBoard, SLOT( showFleets() ), actionCollection(), "game_fleets" );
    fleetAction->setEnabled(false);
    toolBar()->setBarPos( KToolBar::Left );
    toolBar()->setMovingEnabled( false );
}

void
MainWindow::setupGameBoard()
{
    gameBoard = new GameBoard( this );
    setCentralWidget(gameBoard);

    connect( gameBoard, SIGNAL( newGameState( GameState )), this, SLOT( gameStateChange( GameState ) ) );
}


void
MainWindow::gameStateChange( GameState newState )
{
    endAction->setEnabled( gameBoard->isGameInProgress() );
    measureAction->setEnabled( newState==SOURCE_PLANET );
    standingAction->setEnabled( newState==SOURCE_PLANET );
    fleetAction->setEnabled( newState==SOURCE_PLANET );
}




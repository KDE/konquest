#include <config.h>

#include <QPushButton>
#include <QLabel>

#include <kapplication.h>
#include <klocale.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kstdgameaction.h>
#include <kicon.h>
#include <kstatusbar.h>

#include "version.h"
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
    statusBarText = new QLabel(i18n("Galactic Conquest"));
    statusBar()->addWidget(statusBarText);
    
    resize(600, 650);
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
    measureAction = new KAction(KIcon("ruler"),  i18n("&Measure Distance"), actionCollection(), "game_measure" );
    connect(measureAction, SIGNAL(triggered(bool)), gameBoard, SLOT( measureDistance() ));
    measureAction->setEnabled(false);
    standingAction = new KAction(KIcon("help"),  i18n("&Show Standings"), actionCollection(), "game_scores" );
    connect(standingAction, SIGNAL(triggered(bool)), gameBoard, SLOT( showScores() ));
    standingAction->setEnabled(false);
    fleetAction = new KAction(KIcon("launch"),  i18n("&Fleet Overview"), actionCollection(), "game_fleets" );
    connect(fleetAction, SIGNAL(triggered(bool)), gameBoard, SLOT( showFleets() ));
    fleetAction->setEnabled(false);
	addToolBar ( Qt::LeftToolBarArea, toolBar() );
    toolBar()->setMovable(false);
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
    statusBarText->setText(i18n("Turn #: %1 of %2", gameBoard->turnNumber(), gameBoard->lastTurn()));
}

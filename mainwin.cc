#include <kapplication.h>
#include <klocale.h>
#include <kglobal.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <qpushbutton.h>

#include <config.h>

#include "version.h"
#include "gamecore.h"
#include "mainwin.h"
#include "mainwin.moc"
#include "gameboard.h"
#include <kaction.h>
#include <kstdaction.h>
#include <kstdgameaction.h>

// KonquestMainWindow


MainWindow::MainWindow( const char *name) : KMainWindow( 0, name )
{

    setCaption( i18n("GNU-Lactic Conquest") );

    setupGameBoard();
    setupKAction();

    resize( 600,550 );
    setFixedSize( 600, 550 );
    show();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::setupKAction()
{
    KAction* a;
    KStdGameAction::gameNew( gameBoard, SLOT( startNewGame() ), actionCollection() );
    KStdGameAction::quit( kapp, SLOT( quit() ), actionCollection() );
    a = KStdGameAction::end( gameBoard, SLOT( shutdownGame() ), actionCollection() );
    a->setEnabled(false);

    //AB: there is no icon for disabled - KToolBar::insertButton shows the
    //different state - KAction not :-(
    a = new KAction( i18n("&Measure Distance"), "ruler", 0, gameBoard, SLOT( measureDistance() ), actionCollection(), "game_measure" );
    a->setEnabled(false);
    a = new KAction( i18n("&Show Standings"), "help", 0, gameBoard, SLOT( showScores() ), actionCollection(), "game_scores" );
    a->setEnabled(false);
    createGUI( "konquestui.rc" );
    
    toolBar()->setBarPos( KToolBar::Left );
    toolBar()->enableMoving( false );
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

    if( gameBoard->isGameInProgress() ) {
        actionCollection()->action(KStdGameAction::stdName(KStdGameAction::End))->setEnabled(true);
    } else {
        actionCollection()->action(KStdGameAction::stdName(KStdGameAction::End))->setEnabled(false);
    }

    switch( newState ) {

    case SOURCE_PLANET:
        actionCollection()->action("game_measure")->setEnabled(true);
        actionCollection()->action("game_scores")->setEnabled(true);
        break;

    default:
        actionCollection()->action("game_measure")->setEnabled(false);
        actionCollection()->action("game_scores")->setEnabled(false);
        break;

    }


}




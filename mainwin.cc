#include <kapp.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <qpushbt.h>

#include <iostream.h>

#include <config.h>

#include "version.h"
#include "gamecore.h"
#include "mainwin.h"
#include "mainwin.moc"
#include "gameboard.h"

// KonquestMainWindow


MainWindow::MainWindow( const char *name) : KTopLevelWidget( name )
{

    setCaption( i18n("GNU-Lactic Conquest") );

    setupGameBoard();
    setupMenu();
    setupToolBar();

    resize( 600,550 );
    setFixedSize( 600, 550 );
    show();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::setupMenu()
{
    QPopupMenu *game_menu = new QPopupMenu();
    game_menu->insertItem( i18n("&New Game"), gameBoard, SLOT( startNewGame()  ) );
    game_menu->insertSeparator();
    game_menu->insertItem( i18n("&Quit"), qApp, SLOT( quit() ) );

    QString about;
    about.sprintf(i18n("Version %s\nCopyright (c) 1998 by\nThe Gnu-Lactic Conquest Project\n\n"
        "KDE version by Russ Steffen <rsteffen@ia.net>\n"
        "See http://www.ia.net/~rsteffen/konquest.html for more info"),
	KONQUEST_VERSION);
    
    QPopupMenu *help_menu = KApplication::getKApplication()->getHelpMenu( true, about );
    
    menubar = new KMenuBar( this );
    menubar->insertItem( i18n("&Game"), game_menu );
    menubar->insertSeparator( -1 );
    menubar->insertItem( i18n("&Help"), help_menu );

    menubar->enableMoving( false );
    
    setMenu( menubar );
}

void
MainWindow::setupToolBar()
{
    toolbar = new KToolBar( this );

    KIconLoader *iconloader = KApplication::getKApplication()->getIconLoader();

    QPixmap pixmap1 = iconloader->loadApplicationIcon( "stop.xpm" );
    QPixmap pixmap2 = iconloader->loadApplicationIcon( "reload.xpm" );
    QPixmap pixmap3 = iconloader->loadApplicationIcon( "ruler.xpm" );
    QPixmap pixmap4 = iconloader->loadApplicationIcon( "help.xpm" );

    toolbar->insertButton( pixmap2, 1,
                           SIGNAL( clicked() ), gameBoard, SLOT( startNewGame() ),
                           true, i18n("New Game"), -1 );

    toolbar->insertButton( pixmap1, 2,
                           SIGNAL( clicked() ), gameBoard, SLOT( shutdownGame() ),
                           false, i18n("End Game"), -1 );

    toolbar->insertSeparator(-1);

    toolbar->insertButton( pixmap3, 3,
                           SIGNAL( clicked() ), gameBoard, SLOT( measureDistance() ),
                           false, i18n("Measure Distance"), -1 );

    toolbar->insertButton( pixmap4, 4,
                           SIGNAL( clicked() ), gameBoard, SLOT( showScores() ),
                           false, i18n("Show Standings"), -1 );

    toolbar->setBarPos( KToolBar::Left );
    toolbar->enableMoving( false );
    addToolBar( toolbar );
}

void
MainWindow::setupGameBoard()
{
    gameBoard = new GameBoard( this );
    setView(gameBoard);

    connect( gameBoard, SIGNAL( newGameState( GameState )), this, SLOT( gameStateChange( GameState ) ) );
}


void
MainWindow::gameStateChange( GameState newState )
{

    if( gameBoard->isGameInProgress() ) {
        toolbar->setItemEnabled( 2, true );
    } else {
	toolbar->setItemEnabled( 2, false );
    }

    switch( newState ) {

    case SOURCE_PLANET:
        toolbar->setItemEnabled( 3, true );
        toolbar->setItemEnabled( 4, true );
        break;
        
    default:
        toolbar->setItemEnabled( 3, false );
        toolbar->setItemEnabled( 4, false );
        break;

    }

    
}



#include <qlayout.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qslider.h>
#include <qevent.h>
#include <qkeycode.h>
#include <qlistbox.h>
#include <qpushbt.h>
#include <qlined.h>
#include <kmsgbox.h>
#include <kapp.h>
#include <kslider.h>

#include "newgamedlg.h"
#include "newgamedlg.moc"

#include <iostream.h>

/*************************************************************************
 New Game Dialog Members
 ************************************************************************/

NewGameDlg::NewGameDlg( QWidget *parent, Map *pmap, PlayerList *players,
                        Player *neutralPlayer, PlanetList *planets )
    : QDialog( parent, 0, true ),
      plrList(players), plnetList(planets), neutral(neutralPlayer),
      map(pmap)
{

    // Create widgets
    QLabel *mapLabel = new QLabel( i18n("Preview Map"), this );
    mapLabel->setMinimumSize( mapLabel->sizeHint() );
    miniMap = new MiniMap( map, this );

    rejectMapBtn = new QPushButton( i18n("&Reject Map"), this );
    rejectMapBtn->setFixedSize( rejectMapBtn->sizeHint() );
    
    playerList = new QListBox( this );
    playerList->setMinimumSize( 0, 150 );
    
    QLabel *listLabel = new QLabel( playerList, i18n("&Player List"), this );
    listLabel->setFixedSize( listLabel->size() );

    newPlayer = new QLineEdit( this );
    newPlayer->setMaxLength( 8 );
    newPlayer->setMinimumSize(newPlayer->sizeHint());
    
    addPlayer = new QPushButton( i18n("&Add"), this );
    addPlayer->setFixedSize( addPlayer->sizeHint() );

    deletePlayer = new QPushButton( i18n("&Delete"), this );
    deletePlayer->setFixedSize( addPlayer->sizeHint() );
    
    clearList = new QPushButton( i18n("&Clear"), this );
    clearList->setFixedSize( clearList->sizeHint() );

    neutralPlanets = new KSlider( 1, 35, 1, 1, KSlider::Horizontal, this );
    neutralPlanets->setMinimumHeight(neutralPlanets->sizeHint().height());

    neutralPlanetLbl = new QLabel("Number of Neutral Planets: 99",this);
    neutralPlanetLbl->setMinimumSize(neutralPlanetLbl->sizeHint());

    turnCount = new KSlider( 5, 40, 1, 15, KSlider::Horizontal, this );
    turnCount->setTickmarks( KSlider::Below );
    turnCount->setMinimumHeight(turnCount->sizeHint().height());

    turnCountLbl = new QLabel("Number of turns: 99", this);
    turnCountLbl->setMinimumSize(turnCountLbl->sizeHint());
    
    okBtn = new QPushButton( i18n("&Start Game"), this );
    okBtn->setFixedSize( okBtn->sizeHint() );
    okBtn->setEnabled(false);

    cancelBtn = new QPushButton( i18n("&Cancel"), this );
    cancelBtn->setFixedSize( cancelBtn->sizeHint() );


    // Layout Dialog

    QBoxLayout *mainLayout = new QVBoxLayout( this );

    QBoxLayout *mainHLayout = new QHBoxLayout;
    QBoxLayout *btnRow = new QHBoxLayout(10);

    QBoxLayout *mapColumn = new QVBoxLayout;

    QBoxLayout *playerColumn = new QVBoxLayout(5);

    QBoxLayout *playerEditRow = new QHBoxLayout;

    mainLayout->addLayout( mainHLayout, 2 );

    mainHLayout->addSpacing( 5 );
    mainHLayout->addLayout( playerColumn, 2 );
    mainHLayout->addSpacing( 5 );
    mainHLayout->addLayout( mapColumn );
    mainHLayout->addSpacing(5);
    
    mapColumn->addSpacing( 5 );
    mapColumn->addWidget( mapLabel, 0, AlignLeft );
    mapColumn->addWidget( miniMap, 0, AlignLeft );
    mapColumn->addSpacing( 5 );
    mapColumn->addWidget( rejectMapBtn, 0, AlignCenter );
    mapColumn->addStretch(1);
    
    playerColumn->addWidget( listLabel, 0, AlignLeft );
    playerColumn->addWidget( playerList, 1, AlignLeft );
    playerColumn->addLayout( playerEditRow );

    playerEditRow->addWidget( newPlayer, 1 );
    playerEditRow->addWidget( addPlayer );
    playerEditRow->addWidget( deletePlayer );
    playerEditRow->addWidget( clearList );

    mainLayout->addStretch(1);

    mainLayout->addWidget( neutralPlanetLbl, 1, AlignLeft );
    mainLayout->addWidget( neutralPlanets, 1 );
    mainLayout->addWidget( turnCountLbl, 1, AlignLeft );
    mainLayout->addWidget( turnCount, 1 );
    
    mainLayout->addStretch(1);

    mainLayout->addSpacing(10);
    mainLayout->addLayout( btnRow, 0 );
    mainLayout->addSpacing( 5 );

    btnRow->addStrut( 40 );
    btnRow->addWidget( okBtn );
    btnRow->addWidget( cancelBtn );
    btnRow->addStretch( 1 );

    mainLayout->activate();

    // Connect controls
    
    connect( okBtn, SIGNAL( clicked() ), this, SLOT( startGame() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( neutralPlanets, SIGNAL( valueChanged(int) ), this, SLOT( changeNeutralPlanets(int)) );
    connect( turnCount, SIGNAL( valueChanged(int) ), this, SLOT( changeTurnCount(int)) );
    connect( addPlayer, SIGNAL( clicked() ), this, SLOT( addNewPlayer() ) );
    connect( newPlayer, SIGNAL( returnPressed() ), this, SLOT( addNewPlayer() ) );
    connect( deletePlayer, SIGNAL( clicked() ), this, SLOT( removePlayer() ) );
    connect( clearList, SIGNAL( clicked() ), this, SLOT( clearPlayerList() ) );
    connect( rejectMapBtn, SIGNAL( clicked() ), this, SLOT( rejectMap() ) );

    newPlayer->setFocus();

    changeTurnCount( 15 );
    changeNeutralPlanets( 1 );

    setMaximumSize( 490, 320 );
    setMinimumSize( 490, 320 );
    resize( 490, 320 );
}

void
NewGameDlg::changeNeutralPlanets( int newValue )
{
    QString newText;

    newText.sprintf( i18n("Number of Neutral Planets: %2d"), newValue );

    neutralPlanetLbl->setText( newText );

    updateMiniMap();
}

void
NewGameDlg::changeTurnCount( int newTurnCount )
{
    QString newText;

    newText.sprintf( i18n("Number of turns: %2d"), newTurnCount );

    turnCountLbl->setText( newText );
}

void
NewGameDlg::addNewPlayer()
{
    // Is there room for a new player
    if( playerList->count() >= MAX_PLAYERS )
        return;

    // Is the name empty
    if( !strcmp(newPlayer->text(), "" ) )
        return;

    // Does the name already exist in the list
    for( unsigned int x = 0; x < playerList->count(); x++ ) {
        if( !strcmp(newPlayer->text(), playerList->text(x) ) )
            return;
    }

    // Insert the player
    playerList->insertItem( newPlayer->text() );
    newPlayer->setText( "" );

    if( playerList->count() > 1)
       okBtn->setEnabled(true);

    // update the map and game objects
    updateMiniMap();
}

void
NewGameDlg::removePlayer()
{
    int player = playerList->currentItem();

    if( player >= 0 ) {
        playerList->removeItem( player );
    }
    if( playerList->count() < 2)
       okBtn->setEnabled(false);

    updateMiniMap();
}

void
NewGameDlg::clearPlayerList()
{
    playerList->clear();

    updateMiniMap();
}

void
NewGameDlg::startGame()
{
    if( playerList->count() > 1 ) {
        accept();
    }
}

void
NewGameDlg::rejectMap()
{
    map->clearMap();

    // clear out the planet list
    Planet *planet;
    planet = plnetList->first();
    for( planet = plnetList->take(); planet != 0; planet = plnetList->take() ) {
        delete planet;
    }

    // relayout the map
    map->populateMap( *plrList, neutral,
                      neutralPlanets->value(),
                      *plnetList );

}

void
NewGameDlg::updateMiniMap( void )
{
    QColor PlayerColors[MAX_PLAYERS] = { QColor( 130, 130, 255 ), yellow, red, green, white };


    // Clear map,, player and planet lists
    map->clearMap();

    Planet *planet;
    planet = plnetList->first();
    for( planet = plnetList->take(); planet != 0; planet = plnetList->take() ) {
        delete planet;
    }

    Player *player;
    player = plrList->first();
    for( player = plrList->take(); player != 0; player = plrList->take() ) {
        delete player;
    }

    // Make player list
    for( unsigned int plrNum = 0; plrNum < playerList->count(); plrNum++ ) {
        QString plrName( playerList->text( plrNum ) );
        plrList->append( Player::createPlayer( plrName, PlayerColors[plrNum], plrNum ));
    }

    // make the planets
    map->populateMap( *plrList, neutral,
                      neutralPlanets->value(),
                      *plnetList );

}

int
NewGameDlg::turns()
{
    return turnCount->value();
}

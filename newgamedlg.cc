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

/*************************************************************************
 New Game Dialog Members
 ************************************************************************/

NewGameDlg::NewGameDlg( QWidget *parent, Map *pmap, PlayerList *players,
                        Player *neutralPlayer, PlanetList *planets )
    : plrList(players), plnetList(planets), neutral(neutralPlayer),
    map(pmap), QDialog( parent, 0, true  )
{

    // Create widgets
    QLabel *mapLabel = new QLabel( i18n("Preview Map"), this );
    miniMap = new MiniMap( map, this );

    rejectMapBtn = new QPushButton( i18n("&Reject Map"), this );
    rejectMapBtn->setFixedSize( rejectMapBtn->sizeHint() );
    
    playerList = new QListBox( this );
    playerList->setMinimumSize( 0, 150 );
    playerList->setMaximumSize( 32767, 150 );
    
    QLabel *listLabel = new QLabel( playerList, i18n("&Player List"), this );
    listLabel->setFixedSize( listLabel->size() );

    newPlayer = new QLineEdit( this );
    newPlayer->setMaxLength( 8 );
    
    addPlayer = new QPushButton( i18n("&Add"), this );
    addPlayer->setFixedSize( addPlayer->sizeHint() );

    deletePlayer = new QPushButton( i18n("&Delete"), this );
    deletePlayer->setFixedSize( addPlayer->sizeHint() );
    
    clearList = new QPushButton( i18n("&Clear"), this );
    clearList->setFixedSize( clearList->sizeHint() );

    neutralPlanets = new KSlider( 1, 35, 1, 1, KSlider::Horizontal, this );
    neutralPlanets->setTickmarks( KSlider::Below );

    neutralPlanetLbl = new QLabel( this );
    neutralPlanetLbl->setMaximumSize( 32767, 15 );
    neutralPlanetLbl->setMinimumSize( 0, 15 );

    turnCount = new KSlider( 5, 40, 1, 15, KSlider::Horizontal, this );
    turnCount->setTickmarks( KSlider::Below );

    turnCountLbl = new QLabel( this );
    turnCountLbl->setMaximumSize( 32767, 15 );
    turnCountLbl->setMinimumSize( 0, 15 );
    
    okBtn = new QPushButton( i18n("&Start Game"), this );
    okBtn->setFixedSize( okBtn->sizeHint() );

    cancelBtn = new QPushButton( i18n("&Cancel"), this );
    cancelBtn->setFixedSize( cancelBtn->sizeHint() );


    // Layout Dialog

    QBoxLayout *mainLayout = new QVBoxLayout( this );

    QBoxLayout *mainHLayout = new QHBoxLayout;
    QBoxLayout *btnRow = new QHBoxLayout;

    QBoxLayout *mapColumn = new QVBoxLayout;

    QBoxLayout *playerColumn = new QVBoxLayout;

    QBoxLayout *playerEditRow = new QHBoxLayout;
    QBoxLayout *sliderRow = new QHBoxLayout;
    QBoxLayout *sliderRow2 = new QHBoxLayout;


    mainLayout->addLayout( mainHLayout );

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
    mapColumn->addStretch();
    
    playerColumn->addWidget( listLabel, 0, AlignLeft );
    playerColumn->addWidget( playerList, 0, AlignLeft );
    playerColumn->addLayout( playerEditRow );

    playerEditRow->addStrut( 20 );
    playerEditRow->addWidget( newPlayer, 1 );
    playerEditRow->addWidget( addPlayer );
    playerEditRow->addWidget( deletePlayer );
    playerEditRow->addWidget( clearList );

    playerColumn->addWidget( neutralPlanetLbl, 1, AlignLeft );
    playerColumn->addLayout( sliderRow );

    sliderRow->addStrut( 15 );
    sliderRow->addWidget( neutralPlanets, 1 );

    playerColumn->addWidget( turnCountLbl, 1, AlignLeft );
    playerColumn->addLayout( sliderRow2 );

    sliderRow2->addStrut( 15 );
    sliderRow2->addWidget( turnCount, 1 );
    
    playerColumn->addStretch();

    mainLayout->addStretch(1);
    mainLayout->addLayout( btnRow, 0 );
    mainLayout->addSpacing( 5 );

    btnRow->addStrut( 40 );
    btnRow->addWidget( okBtn );
    btnRow->addStretch( 1 );
    btnRow->addWidget( cancelBtn );


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

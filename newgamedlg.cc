#include <qlayout.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qslider.h>
#include <qevent.h>
#include <qkeycode.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <kapplication.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kstdguiitem.h>

#include "newgamedlg.h"
#include "newgamedlg.moc"


/*************************************************************************
 New Game Dialog Members
 ************************************************************************/

NewGameDlg::NewGameDlg( QWidget *parent, Map *pmap, PlayerList *players,
                        Player *neutralPlayer, PlanetList *planets )
    : QDialog( parent, 0, true ),
      plrList(players), plnetList(planets), neutral(neutralPlayer),
      map(pmap)
{
    setCaption(kapp->makeStdCaption(""));
    // Create widgets
    QLabel *mapLabel = new QLabel( i18n("Preview map:"), this );
    mapLabel->setMinimumSize( mapLabel->sizeHint() );
    miniMap = new MiniMap( map, this );

    rejectMapBtn = new QPushButton( i18n("&Reject Map"), this );
    rejectMapBtn->setFixedSize( rejectMapBtn->sizeHint() );

    playerList = new QListBox( this );
    playerList->setMinimumSize( 0, 150 );

    QLabel *listLabel = new QLabel( playerList, i18n("&Player list:"), this );
    listLabel->setFixedSize( listLabel->size() );

    addAiPlayer = new QPushButton( i18n("&Add Computer Player"), this );
    addAiPlayer->setMinimumSize( addAiPlayer->sizeHint() );

    newPlayer = new QLineEdit( this );
    newPlayer->setMaxLength( 8 );
    newPlayer->setMaximumWidth(100);

    addPlayer = new QPushButton( i18n("&Add"), this );
    addPlayer->setMinimumSize( addPlayer->sizeHint() );

    deletePlayer = new QPushButton( i18n("&Delete"), this );
    deletePlayer->setMinimumSize( deletePlayer->sizeHint() );

    clearList = new KPushButton( KStdGuiItem::clear(), this );
    clearList->setMinimumSize( clearList->sizeHint() );

    neutralPlanets = new QSlider( 1, 35, 1, 1, Qt::Horizontal, this );
    neutralPlanets->setMinimumHeight(neutralPlanets->sizeHint().height());

    neutralPlanetLbl = new QLabel("Number of neutral planets: 99",this);
    neutralPlanetLbl->setMinimumSize(neutralPlanetLbl->sizeHint());

    turnCount = new QSlider( 5, 40, 1, 15, Qt::Horizontal, this );
    turnCount->setTickmarks( QSlider::Below );
    turnCount->setMinimumHeight(turnCount->sizeHint().height());

    turnCountLbl = new QLabel("Number of turns:", this);
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

    QBoxLayout *playerEditRow = new QVBoxLayout( );

    mainLayout->addLayout( mainHLayout, 2 );

    mainHLayout->addSpacing( 5 );
    mainHLayout->addLayout( playerColumn, 2 );
    mainHLayout->addLayout( playerEditRow );
    mainHLayout->addSpacing( 20 );
    mainHLayout->addLayout( mapColumn );
    mainHLayout->addSpacing(5);

    mapColumn->addSpacing( 5 );
    mapColumn->addWidget( mapLabel, 0, AlignLeft );
    mapColumn->addWidget( miniMap, 0, AlignLeft );
    mapColumn->addSpacing( 5 );
    mapColumn->addWidget( rejectMapBtn, 0, AlignCenter );
    mapColumn->addStretch(1);

    playerColumn->addWidget( listLabel, 0, AlignLeft );
    playerColumn->addWidget( newPlayer, 1, AlignLeft );
    playerColumn->addWidget( playerList, 1, AlignLeft );

    playerEditRow->addSpacing( 35 );
    playerEditRow->addWidget( addPlayer, 0 );
    playerEditRow->addWidget( addAiPlayer, 0 );
    playerEditRow->addWidget( deletePlayer, 0 );
    playerEditRow->addWidget( clearList, 0, AlignTop );

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
    connect( addAiPlayer, SIGNAL( clicked() ), this, SLOT( addNewAiPlayer() ) );
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

    newText = i18n("Number of neutral planets: %1").arg( newValue );

    neutralPlanetLbl->setText( newText );

    updateMiniMap();
}

void
NewGameDlg::changeTurnCount( int newTurnCount )
{
    QString newText;

    newText = i18n("Number of turns: %1").arg( newTurnCount );

    turnCountLbl->setText( newText );
}

void
NewGameDlg::addNewPlayer()
{
    // Is there room for a new player
    if( playerList->count() >= MAX_PLAYERS )
        return;

    // Is the name empty
    if( newPlayer->text().isEmpty() )
        return;

    // Does the name already exist in the list
    for( unsigned int x = 0; x < playerList->count(); x++ ) {
        if( newPlayer->text() == playerList->text(x) )
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

void NewGameDlg::addNewAiPlayer() {

    // Is there room for a new player
    if( playerList->count() >= MAX_PLAYERS )
        return;

    // Does the name already exist in the list
    for( unsigned int x = 0; x < playerList->count(); x++ ) {
        if( newPlayer->text() == playerList->text(x) )
            return;
    }

    // Insert the player
    QString name;
    int aiNumber = 0;

    for( unsigned int plrNum = 0; plrNum < playerList->count(); plrNum++ ) {
        QString plrName( playerList->text( plrNum ) );
        if (plrName.startsWith("Comp"))
        	aiNumber++;
    }

    name = i18n("Generated AI player name", "Comp%1").arg(aiNumber+1);
    playerList->insertItem( name );

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
    QColor PlayerColors[MAX_PLAYERS] = { QColor( 130, 130, 255 ), yellow, red, green,
    		white, cyan, magenta,  QColor( 131, 153, 128), QColor( 235, 153, 46 ),
		QColor( 106, 157, 104 ) };


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
        bool ai = false;
        if (plrName.startsWith("Comp"))
        	ai = true;
        plrList->append( Player::createPlayer( plrName, PlayerColors[plrNum], plrNum, ai ));
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

#include <qlayout.h>
#include <qcolor.h>
#include <qlabel.h>
#include <qslider.h>
#include <qevent.h>
#include <qkeycode.h>
#include <qlistbox.h>
#include <qpushbt.h>
#include <qlined.h>
#include <qvalidator.h>
#include <kmsgbox.h>
#include <kapp.h>
#include <kslider.h>
#include <kiconloader.h>

#include <ctype.h>
#include <math.h>
#include <iostream.h>

#include "gamecore.h"

#include "int_validator.h"
#include "newgamedlg.h"
#include "gameenddlg.h"
#include "scoredlg.h"
#include "gameboard.h"
#include "gameboard.moc"


/*********************************************************************
 Game Board
*********************************************************************/
GameBoard::GameBoard( QWidget *parent )
    : QWidget( parent ), gameInProgress( false ), gameState( NONE )
{
    QColorGroup cg( white, black, green.light(), green.dark(), green, green.dark(75), green.dark() );
    QPalette palette( cg, cg, cg );

    KIconLoader *iconloader = KApplication::getKApplication()->getIconLoader();

    neutralPlayer = Player::createNeutralPlayer();
    map = new Map;

    //********************************************************************
    // Create the widgets in the main window
    //********************************************************************
    mapWidget = new ConquestMap( map, this );
    planetInfo = new PlanetInfo( this, palette );
    gameMessage = new QLabel( this );
    gameMessage->setPalette( palette );
    turnCounter = new QLabel( this );
    turnCounter->setPalette( palette );
    turnCounter->setText( "Turn" );

    endTurn = new QPushButton( i18n("End Turn"), this );
    endTurn->setFixedSize( endTurn->sizeHint() );
    endTurn->setPalette( palette );

    shipCountEdit = new QLineEdit( this );
    IntValidator *v = new IntValidator( 1, 32767, this );
    shipCountEdit->setValidator( v );
    shipCountEdit->setMinimumSize( 40, 0 );
    shipCountEdit->setMaximumSize( 32767, 40 );
    shipCountEdit->setEnabled(false);
    shipCountEdit->setPalette( palette );
    shipCountEdit->setEchoMode( QLineEdit::Password );

    splashScreen = new QLabel( this );
    splashScreen->setPixmap( iconloader->loadApplicationIcon( IMAGE_SPLASH ));
    splashScreen->setGeometry( 0, 0, 600, 550 );


    setFocusPolicy( StrongFocus );
    setFocus();

    //********************************************************************
    // Layout the main window
    //********************************************************************
    QHBoxLayout *layout1 = new QHBoxLayout( this );
    QVBoxLayout *layout2 = new QVBoxLayout;
    QHBoxLayout *layout3 = new QHBoxLayout;
    QVBoxLayout *layout4 = new QVBoxLayout;

    layout1->addLayout( layout2 );
    layout2->addLayout( layout3 );

    layout3->addWidget( gameMessage, 10 );
    layout3->addWidget( shipCountEdit, 1 );
    layout3->addWidget( endTurn, 1 );

    layout2->addSpacing( 5 );
    layout2->addWidget( mapWidget, 0, AlignTop );
    layout2->addStretch( 1 );

    layout1->addSpacing( 5 );
    layout1->addLayout( layout4, 10 );
    
    layout4->addWidget( planetInfo, 1 );
    layout4->addWidget( turnCounter,  1 );
    layout4->addStretch( 1 );

    layout1->addStretch( 1 );

    //**********************************************************************
    // Set up signal/slot connections
    //**********************************************************************
    connect( mapWidget, SIGNAL( planetSelected(Planet *) ), this, SLOT(planetSelected(Planet *)) );
    connect( shipCountEdit, SIGNAL(returnPressed()), this, SLOT(newShipCount()) );
    connect( endTurn, SIGNAL( clicked() ), this, SLOT( nextPlayer() ) );
    connect( mapWidget, SIGNAL( planetHighlighted(Planet *)), planetInfo, SLOT(showPlanet(Planet *)) );

    changeGameBoard( false );
}


//**********************************************************************
// Destructor
//**********************************************************************
GameBoard::~GameBoard()
{
    // Nothing much to do yet
}

//************************************************************************
// Keyboard Event handlers
//************************************************************************
void
GameBoard::keyPressEvent( QKeyEvent *e )
{
    // Check for the escape key
    if( e->key() == Key_Escape ) {
        switch( gameState ) {
        case DEST_PLANET:
        case SHIP_COUNT:
        case RULER_SOURCE:
        case RULER_DEST:
            gameState = SOURCE_PLANET;
            haveSourcePlanet = false;
            haveDestPlanet = false;
            turn();
            break;
        default:
            break;
        }
        return;
    }
    
    if( !isgraph( e->ascii() ) ) {
        e->ignore();
        return;
    }

    PlanetListIterator planetSearch( planets );
    QString planetName;

    planetName += toupper( e->ascii() );

    for(Planet *p = planetSearch.toFirst();
        p != NULL;
        p = ++planetSearch ) {

        if( p->getName() == planetName )
            planetSelected( p );
    }

}

//************************************************************************
// Game engine/state machine
//************************************************************************
void
GameBoard::turn( void )
{
    switch( gameState ) {
    case NONE :
        // stuff for none
        gameState = SOURCE_PLANET;
        haveSourcePlanet = false;
        haveDestPlanet = false;
        haveShipCount = false;
        shipCount = 0;
        mapWidget->unselectPlanet();


        turn();
        setFocus();
        break;

    case SOURCE_PLANET :

        if( haveSourcePlanet ) {
            gameState = DEST_PLANET;

            sourcePlanet->select();
            turn();

        } else {
            shipCountEdit->hide();
            endTurn->setEnabled( true );
            mapWidget->unselectPlanet();

            gameMessage->setText( currentPlayer->current()->getName() +
                                    i18n(": Select source planet...") );
            setFocus();
        }

        break;

    case DEST_PLANET :

        if( haveDestPlanet ) {
            mapWidget->unselectPlanet();
            gameState = SHIP_COUNT;
            turn();

        } else {
            shipCountEdit->hide();
            endTurn->setEnabled( false );
            sourcePlanet->select();
            gameMessage->setText( currentPlayer->current()->getName() +
                                    i18n(": Select destination planet...") );
            setFocus();
        }

        break;

    case SHIP_COUNT:

        if( haveShipCount ) {
            // We now have a complete fleet to send, so send it
            sendAttackFleet();

            shipCountEdit->hide();
            endTurn->setEnabled( true );

            gameState = NONE;
            turn();

            endTurn->setFocus();

        } else {
            gameMessage->setText( currentPlayer->current()->getName() +
                                    i18n(": How many ships?") );

            shipCountEdit->setText( "" );
            shipCountEdit->show();
            shipCountEdit->setEnabled(true);
            shipCountEdit->setFocus();

            endTurn->setEnabled( false );

            mapWidget->unselectPlanet();
        }

        break;
        
    case RULER_SOURCE:
        if( haveSourcePlanet ) {
            gameState = RULER_DEST;
            sourcePlanet->select();
            turn();
        } else {
            shipCountEdit->hide();
            endTurn->setEnabled( true );
            mapWidget->unselectPlanet();

            gameMessage->setText( i18n("Ruler: Select starting planet.") );
            setFocus();
        }

        break;

    case RULER_DEST:
        if( haveDestPlanet ) {
            mapWidget->unselectPlanet();

            // Display the distance between the two planets
            CoreLogic cl;
            double dist = cl.distance( sourcePlanet, destPlanet );

            QString distString;
            distString.setNum( dist, 'f', 2 );
            QString travelTime;
            travelTime.setNum( turnNumber + (int)dist );

            QString msg;
            msg.sprintf(i18n("The distance from Planet %s to Planet %s is %s light years.\n"
                              "A ship leaving this turn will arrive on turn %s"),
                              sourcePlanet->getName().data(), destPlanet->getName().data(),
                              distString.data(), travelTime.data());
            KMsgBox::message( this, i18n("Distance"), msg, KMsgBox::INFORMATION );

            gameState = NONE;
            turn();
        } else {
            gameMessage->setText( i18n("Ruler: Select ending planet.") );
            shipCountEdit->hide();
            endTurn->setEnabled( false );
            sourcePlanet->select();

            setFocus();
        }

        break;

    default:
        break;
    }

    QString turnStr;
    turnStr.sprintf( i18n("Turn #: %2d of %2d"), turnNumber, lastTurn );

    turnCounter->setText( turnStr );

    emit newGameState( gameState );
}
//************************************************************************
// To the end turn processing (resolve combat, etc.)
//************************************************************************
void
GameBoard::nextTurn( void )
{
    resolveShipsInFlight();

    scanForSurvivors();
    
    // advance turn counter
    turnNumber++;

    if( turnNumber == lastTurn ) {
        // Last call
        //KMsgBox::message( this, i18n("Last Turn"), i18n("This is the last turn."), KMsgBox::INFORMATION );

        GameEndDlg *dlg = new GameEndDlg( this );

        if( dlg->exec() ) {
            lastTurn += dlg->addTurns;
        }

        delete dlg;
    }
    
    if( turnNumber > lastTurn ) {
        // Game over, man! Game over.
        cleanupGame();
    };
    
    // update the planets
    PlanetListIterator nextPlanet( planets );
    Planet *planet;

    while( (planet = nextPlanet()) ) {
        planet->turn();
    }

    // Tell the status widget to update itself
    planetInfo->rescanPlanets();
}

//************************************************************************
// determine the fate of the ships in transit
//************************************************************************
void
GameBoard::resolveShipsInFlight( void )
{
    AttackFleetList arrivingShips;
    PlayerListIterator nextPlayer( players );
    Player *plr;

    while( (plr = nextPlayer()) ) {
        AttackFleetListIterator nextFleet( plr->getAttackList() );

        AttackFleet *fleet;

        while( (fleet = nextFleet()) ) {
            double fleetArrivalTurn = floor(fleet->arrivalTurn);

            if( turnNumber == int (fleetArrivalTurn) ) {
                doFleetArrival( fleet );
                plr->getAttackList().removeRef( fleet );
                delete fleet;
            }
        }
    }

}

//************************************************************************
// check to see any players have been eliminated
//************************************************************************
void
GameBoard::scanForSurvivors( void )
{
    PlayerListIterator nextPlayer( players );
    PlayerList activePlayers;
    PlayerList inactivePlayers;
    
    // insert all of the active players into a special
    // list, the deactivate them
    Player *plr;
    while( (plr = nextPlayer()) ) {
        if( plr->isInPlay() ) {
            activePlayers.append( plr );
            plr->setInPlay( false );
        } else {
            inactivePlayers.append( plr );
        }
    }


    // iterate through the list of planets and
    // mark their owners in play
    PlanetListIterator nextPlanet( planets );

    Planet *planet;
    while( (planet = nextPlanet()) ) {
        planet->getPlayer()->setInPlay( true );
    }


    PlayerListIterator nextActivePlayer( activePlayers );
    while( (plr = nextActivePlayer()) ) {
        if( !plr->isInPlay() ) {
            // Player has bitten the dust
            QString msg;
            msg.sprintf(i18n("The once mighty empire of %s has fallen in ruins."),
                        plr->getName().data());
            KMsgBox::message( this, i18n("An Empire has fallen..."),
                              msg, KMsgBox::INFORMATION );
        }
    }

    PlayerListIterator nextInactivePlayer( inactivePlayers );
    while( (plr = nextInactivePlayer()) ) {
        if( plr->isInPlay() ) {
            // Player has bitten the dust
            QString msg;
            msg.sprintf(i18n("The fallen empire of %s has staggered back to life."),
			plr->getName().data());
            KMsgBox::message( this, i18n("Up from the ashes..."),
                              msg, KMsgBox::INFORMATION );
        }
    }
}

//************************************************************************
// handle the arrival of a fleet at a planet
//************************************************************************
void
GameBoard::doFleetArrival( AttackFleet *arrivingFleet )
{
    // Check to see of (fleet owner) == (planet owner)
    // if the planet and fleet owner are the same, then merge the fleets
    // otherwise attack.

    if( (*arrivingFleet->owner) == (*arrivingFleet->destination->getPlayer()) ) {
        arrivingFleet->destination->getFleet().absorb(arrivingFleet);

        QString msg;
        msg.sprintf(i18n("Reinforcements have arrived for planet %s"),
                    arrivingFleet->destination->getName().data());
        KMsgBox::message(this, i18n("Fleet Arrival"),
                         msg, KMsgBox::INFORMATION );
        
    } else {

        // let's get ready to rumble...

        CoreLogic cl;
        AttackFleet &attacker = *arrivingFleet;
        DefenseFleet &defender = arrivingFleet->destination->getFleet();
        Planet &prizePlanet = *(arrivingFleet->destination);

        bool haveVictor = false;
        bool planetHolds = true;

        while( !haveVictor ) {
            double attackerRoll = cl.roll();
            double defenderRoll = cl.roll();

            if( defenderRoll < prizePlanet.getKillPercentage() ) {
                attacker.removeShips( 1 );
            }

            if( attacker.getShipCount() <= 0 ) {
                haveVictor = true;
                planetHolds = true;
                continue;
            }

            if( attackerRoll < attacker.killPercentage ) {
                defender.removeShips( 1 );
                attacker.owner->statEnemyShipsDestroyed( 1 );
            }

            if( defender.getShipCount() <= 0 ) {
                haveVictor = true;
                planetHolds = false;
            }
        }

        if( planetHolds ) {
            prizePlanet.getPlayer()->statEnemyFleetsDestroyed(1);
            QString msg;
            msg.sprintf(i18n("Planet %s has held against an attack from %s"),
                        prizePlanet.getName().data(), attacker.owner->getName().data());
            KMsgBox::message(this, i18n("Planet Holds"),
                             msg, KMsgBox::INFORMATION );
        } else {
            attacker.owner->statEnemyFleetsDestroyed( 1 );

            arrivingFleet->destination->conquer( arrivingFleet );

            QString msg;
            msg.sprintf(i18n("Planet %s has fallen to %s"),
                        prizePlanet.getName().data(), attacker.owner->getName().data());
            KMsgBox::message(this, i18n("Planet Conquered"),
                             msg, KMsgBox::INFORMATION );

        }

    }


    mapWidget->repaint(true);
}

//************************************************************************
// Set up the game board for a new game
//************************************************************************
void
GameBoard::startNewGame()
{
    shutdownGame();

    if( gameInProgress )
        return;

    NewGameDlg *newGame = new NewGameDlg( this, map, &players, neutralPlayer, &planets );


    if( !newGame->exec() )
    {
        delete newGame;
        return;
    }

    changeGameBoard( true );

    planetInfo->setPlanetList(planets);

    shipCountEdit->hide();
    endTurn->setEnabled( true );

    currentPlayer = new PlayerListIterator( players );
    currentPlayer->toFirst();

    endTurn->show();
    gameMessage->show();

    lastTurn = newGame->turns();
    
    turnNumber = 1;
    turn();
    
    delete newGame;
}

//************************************************************************
// Shut down the current game
//************************************************************************
void
GameBoard::shutdownGame()
{
    if( !gameInProgress )
        return;

    int choice = KMsgBox::yesNo( this,
                                 i18n("End Game"),
                                 i18n("Do you wish to retire this game?"),
                                 0, i18n("Yes"), i18n("Cancel") );

    if( choice != 1 )
        return;

    ScoreDlg *scoreDlg = new ScoreDlg( this, i18n("Final Standings"), &players );
    scoreDlg->show();

    cleanupGame();
}

void
GameBoard::cleanupGame( void )
{
    map->clearMap();

    Planet *planet;
    planet = planets.first();
    for( planet = planets.take(); planet != 0; planet = planets.take() ) {
        delete planet;
    }

    Player *player;
    player = players.first();
    for( player = players.take(); player != 0; player = players.take() ) {
        delete player;
    }

    delete currentPlayer;
    currentPlayer = NULL;

    shipCountEdit->hide();
    endTurn->setEnabled( false );

    gameMessage->hide();
    endTurn->hide();

    changeGameBoard( false );
    gameState = NONE;
    emit newGameState(gameState);
}


//************************************************************************
// Player selected a planet
//************************************************************************
void
GameBoard::planetSelected( Planet *planet )
{
    switch( gameState ) {
    case SOURCE_PLANET:
        if( (*planet->getPlayer()) == (*currentPlayer->current()) ) {
            // got a match
            haveSourcePlanet = true;
            sourcePlanet = planet;

            turn();
        }

        break;

    case RULER_SOURCE:
        haveSourcePlanet = true;
        sourcePlanet = planet;
        turn();
        break;

    case DEST_PLANET:
    case RULER_DEST:
        if( planet != sourcePlanet ) {
            // got a match
            haveDestPlanet = true;
            destPlanet = planet;

            turn();
        }

        break;
        
    default:
    case NONE :
        break;
    }

}

//************************************************************************
// Player hit return in the ship count edit box
//************************************************************************
void
GameBoard::newShipCount()
{
    QString temp( shipCountEdit->text() );
    bool ok;
        
    switch( gameState ) {
    case SHIP_COUNT:
        shipCount = temp.toInt(&ok);

        if( ok )
            haveShipCount = true;

        shipCountEdit->setText( "" );
        
        turn();
        break;
        
    default:
        break;
    };
    
}

//**********************************************************************
// transition board from play to non-play
//**********************************************************************
void
GameBoard::changeGameBoard( bool inPlay  )
{
    gameInProgress = inPlay;

    if( gameInProgress ) {
        mapWidget->show();
        planetInfo->show();
        gameMessage->show();
        endTurn->show();
        shipCountEdit->show();
        splashScreen->hide();
        setBackgroundColor( black );
    } else {
        mapWidget->hide();
        planetInfo->hide();
        gameMessage->hide();
        endTurn->hide();
        shipCountEdit->hide();
        splashScreen->show();
        setBackgroundColor( black );
    }

}

//************************************************************************
// Player clicked the 'End Turn' button
//************************************************************************
void
GameBoard::nextPlayer()
{
    // wnd turn and advance to next player
    Player *plr;

    while( (plr = ++(*currentPlayer)) && !(plr->isInPlay()) ) {}

    if( !plr ) {
        // end of player list, new turn
        currentPlayer->toFirst();
        nextTurn();
    }

    if( gameInProgress ) {
        gameState = SOURCE_PLANET;
        turn();
    }
}

//************************************************************************
// A complete set of source, destination planets and ship count has been
// entered, so do something about it
//************************************************************************
void
GameBoard::sendAttackFleet( void )
{
    bool ok;

    ok = currentPlayer->current()->NewAttack( sourcePlanet, destPlanet,
                                                shipCount, turnNumber );

    if( !ok ) {
        KMsgBox::message( this, i18n("Error"), i18n("Not enough ships to send."), KMsgBox::EXCLAMATION  );
    }
}

//************************************************************************
// Toolbar items
//************************************************************************
void
GameBoard::measureDistance()
{
    switch( gameState ) {
    case SOURCE_PLANET:
        gameState = RULER_SOURCE;
        turn();
    default:
        break;
    }
};

void
GameBoard::showScores()
{

    ScoreDlg *scoreDlg = new ScoreDlg( this, "Current Standings", &players );
    scoreDlg->show();

}

/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KONQUEST_GAMEVIEW_H
#define KONQUEST_GAMEVIEW_H


#include <QWidget>

#include "planet.h"
#include "players/player.h"
#include "game.h"


//************************************************************************
// forward declarations
//************************************************************************

class QCheckBox;
class QLabel;
class QPushButton;
class QLineEdit;
class QTextEdit;
class QIntValidator;

class KLocalizedString;

class Player;
class MapView;
class MapScene;
class GameLogic;


struct GameMessage {
    QString  text;
    Player  *sender;
    Player  *receiver;
};


enum GUIState { 
    NONE, 
    SOURCE_PLANET, 
    DEST_PLANET, 
    SHIP_COUNT, 
    RULER_SOURCE, 
    RULER_DEST
};


//************************************************************************
// GameView Widget
//************************************************************************


class GameView : public QWidget
{
    Q_OBJECT

public:
    explicit  GameView( QWidget *parent, Game *game );
    virtual  ~GameView();

protected slots:
    bool  shutdownGame();
    void  planetSelected( Planet * );
    void  newShipCount();
    void  nextPlayer();
    void  standingOrdersClicked();

    //***************************************************************
    // Toolbar items
    //***************************************************************
    void  measureDistance();
    void  showScores();
    void  showFleets();

public slots:
    void  startNewGame();
    void  gameMsg(const KLocalizedString &msg, Player *player = 0,
		  Planet *planet = 0, Player *planetPlayer = 0);
    void  turn();

signals:
    void  newGUIState( GUIState newState );

    //***************************************************************
    // Event Handlers
    //***************************************************************
protected:
    virtual void  keyPressEvent( QKeyEvent * );
    virtual void  resizeEvent ( QResizeEvent * event );

private slots:
    void  gameOver();

private:
    void  changeGameView();
    void  cleanupGame();
    

    //***************************************************************
    // Display Widgets
    //***************************************************************

    MapView       *m_mapWidget;
    MapScene      *m_mapScene;
    QLabel        *m_gameMessage;
    QPushButton   *m_endTurnBtn;
    QLineEdit     *m_shipCountEdit;
    QCheckBox     *m_standingOrder;
    QIntValidator *m_shipValidator;
    QLabel        *m_splashScreen;
    QTextEdit     *m_msgWidget;

    //***************************************************************
    // Game objects
    //***************************************************************

    Game *m_game;

    bool                m_queueMessages;
    QList<GameMessage>  m_messageQueue;
    bool                m_showInformations;

    // States in the user interaction
    // FIXME: Break out into another file?
    bool                haveSourcePlanet;
    Planet             *sourcePlanet;

    bool                haveDestPlanet;
    Planet             *destPlanet;

    bool                haveShipCount, standingOrder;
    int                 shipCount;

    //***************************************************************
    // Game State information
    //***************************************************************
    GUIState            m_guiState;
};

#endif // KONQUEST_GAMEVIEW_H

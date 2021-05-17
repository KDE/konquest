/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
class QDockWidget;
class QLabel;
class QPushButton;
class QLineEdit;
class QTextEdit;
class QIntValidator;

class KLocalizedString;

class Player;
class MapView;
class MapScene;
class StandingsWidget;


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
    explicit  GameView(QWidget *parent, Game *game, QDockWidget *messagesDock, QDockWidget *standingsDock);
    virtual  ~GameView();
    bool  confirmNewGame();

    bool  shutdownGame();
    void  planetSelected( Planet * );
    void  newShipCount();
    void  nextPlayer();
    void  standingOrdersClicked();

    //***************************************************************
    // Toolbar items
    //***************************************************************
    void  measureDistance();
    void  showFleets();

    void  startNewGame();
    void  gameMsg(const KLocalizedString &msg, Player *player = nullptr,
		  Planet *planet = nullptr, Player *planetPlayer = nullptr);
    void  turnPreparation();

Q_SIGNALS:
    void  newGUIState( GUIState newState );

    //***************************************************************
    // Event Handlers
    //***************************************************************
protected:
    void  keyPressEvent( QKeyEvent * ) override;
    void  resizeEvent ( QResizeEvent * event ) override;

private:
    void  gameOver();

    void  changeGameView();
    void  cleanupGame();

    void  turn();

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
    int            m_msgWidgetLastTurn;

    StandingsWidget *m_standingsWidget;

    QDockWidget   *m_messagesDock;
    QDockWidget   *m_standingsDock;

    //***************************************************************
    // Game objects
    //***************************************************************

    Game *m_game;

    bool                m_queueMessages;
    QList<GameMessage>  m_messageQueue;
    bool                m_showInformations;
    bool                m_initCompleted, m_cleanupNeeded;

    // States in the user interaction
    // FIXME: Break out into another file?
    bool                haveSourcePlanet;
    Planet             *sourcePlanet;

    bool                haveDestPlanet;
    Planet             *destPlanet;

    bool                haveShipCount, standingOrder;
    long long           shipCount;

    //***************************************************************
    // Game State information
    //***************************************************************
    GUIState            m_guiState;
};

#endif // KONQUEST_GAMEVIEW_H

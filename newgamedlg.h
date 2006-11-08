#ifndef _NEWGAMEDLG_H_
#define _NEWGAMEDLG_H_

#include <kdialog.h>

#include "planet.h"
#include "minimapview.h"
#include "ui_newGameDlg_ui.h"
#include "player.h"

// Maximum Number of Players
#define MAX_PLAYERS 10

class NewGameDlgUI : public QWidget, public Ui::NewGameDlgUI
{
public:
  explicit NewGameDlgUI( QWidget *parent ) : QWidget( parent ) {
    setupUi( this );
  }
};


/*************************************************************************
 New Game Dialog
 ************************************************************************/

class NewGameDlg : public KDialog
{
    Q_OBJECT

public:
    NewGameDlg( QWidget *parent, Map *map, QList<Player *> *playerList,
                Player *neutralPlayer, QList<Planet *> *planetList );

    int turns( void );

    void save();

protected slots:
    void slotPlayerCount(int playerCount);
    void slotNewMap();
    void slotTurns();
    void slotNewPlayer();
    void slotAddPlayer();
    void slotDefault();
    void slotOk();

private:
    void init();
    void updateMiniMap();
    void updateLabels();
    void setPlayerCount(int playerCount);

private:
    QList<Player *> *plrList;
    QList<Planet *> *plnetList;
    Player *neutral;
    Map *map;

    NewGameDlgUI *w;
};

#endif

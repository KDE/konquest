#ifndef _NEWGAMEDLG_H_
#define _NEWGAMEDLG_H_

#include <kdialogbase.h>

#include "gamecore.h"
#include "minimap.h"

class NewGameDlgUI;

/*************************************************************************
 New Game Dialog
 ************************************************************************/

class NewGameDlg : public KDialogBase
{
    Q_OBJECT

public:
    NewGameDlg( QWidget *parent, Map *map, PlayerList *playerList,
                Player *neutralPlayer, PlanetList *planetList );

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
    PlayerList *plrList;
    PlanetList *plnetList;
    Player *neutral;
    Map *map;

    NewGameDlgUI *w;
};

#endif

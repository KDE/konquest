#ifndef _NEWGAMEDLG_H_
#define _NEWGAMEDLG_H_

#include <qdialog.h>

#include "gamecore.h"
#include "minimap.h"

/*************************************************************************
 New Game Dialog
 ************************************************************************/

class NewGameDlg : public QDialog
{
    Q_OBJECT;

public:
    NewGameDlg( QWidget *parent, Map *map, PlayerList *playerList,
                Player *neutralPlayer, PlanetList *planetList );

    int turns( void );
    
public slots:
    void changeNeutralPlanets( int );
    void changeTurnCount( int );
    void addNewPlayer();
    void removePlayer();
    void clearPlayerList();
    void startGame();
    void rejectMap();
    
private:
    void updateMiniMap( void );

    PlayerList *plrList;
    PlanetList *plnetList;
    Player *neutral;
    Map *map;

    QListBox *playerList;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QLineEdit *newPlayer;
    QPushButton *addPlayer;
    QPushButton *deletePlayer;
    QPushButton *clearList;
    QSlider *neutralPlanets;
    QSlider *turnCount;
    QLabel *neutralPlanetLbl;
    QLabel *turnCountLbl;
    MiniMap *miniMap;
    QPushButton *rejectMapBtn;

};

#endif

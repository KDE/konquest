#ifndef _NEWGAMEDLG_H_
#define _NEWGAMEDLG_H_


#include <kdialog.h>

#include "planet.h"
#include "minimapview.h"
#include "ui_newGameDialog.h"
#include "player.h"


// Maximum Number of Players
#define MAX_PLAYERS 10


class NewGameDlgUI : public QWidget, public Ui::newGameDialog
{
public:
    explicit NewGameDlgUI( QWidget *parent ) : QWidget( parent ) {
	setupUi( this );
    }
	~NewGameDlgUI() {}
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
    ~NewGameDlg() {}

    void  save();

protected slots:
    void  slotNewMap();
    void  slotAddPlayer();
    void  slotRemovePlayer();
    void  slotOk();

private:
    void  init();

private:
    Map              *m_map;
    QList<Player *>  *m_players;
    QList<Planet *>  *m_planets;
    Player           *m_neutral;

    NewGameDlgUI     *m_w;
};

#endif

#ifndef _SCOREDLG_H_
#define _SCOREDLG_H_


#include <QTableWidget>
#include <KDialog>

#include "player.h"


class ScoreDlg : public KDialog
{

public:
    ScoreDlg( QWidget *parent, const QString& title, 
	      QList<Player *> *players );
    ~ScoreDlg();

private:
    void init();

    QList<Player *>  *m_players;
    QTableWidget     *m_scoreTable;

};

#endif // _SCOREDLG_H_


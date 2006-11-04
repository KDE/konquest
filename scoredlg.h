#ifndef _SCOREDLG_H_
#define _SCOREDLG_H_

#include <QTableWidget>

#include <QDialog>

#include "player.h"

class ScoreDlg : public QDialog
{

public:
    ScoreDlg( QWidget *parent, const QString& title, PlayerList *players );

private:
    void init();

    PlayerList *plrList;
    QTableWidget *scoreTable;

};

#endif // _SCOREDLG_H_


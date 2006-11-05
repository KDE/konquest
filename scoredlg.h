#ifndef _SCOREDLG_H_
#define _SCOREDLG_H_

#include <QTableWidget>

#include <QDialog>

#include "player.h"

class ScoreDlg : public QDialog
{

public:
    ScoreDlg( QWidget *parent, const QString& title, QList<Player *> *players );

private:
    void init();

    QList<Player *> *plrList;
    QTableWidget *scoreTable;

};

#endif // _SCOREDLG_H_


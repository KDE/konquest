#ifndef _SCOREDLG_H_
#define _SCOREDLG_H_

#include <qdialog.h>

#include "gamecore.h"

class QListView;

class ScoreDlg : public QDialog
{
    Q_OBJECT
public:
    ScoreDlg( QWidget *parent, const QString& title, PlayerList *players );

private:
    void init( void );

    PlayerList *plrList;
    QListView *scoreTable;

};

#endif // _SCOREDLG_H_


#ifndef _SCOREDLG_H_
#define _SCOREDLG_H_

#include <qdialog.h>

#include "gamecore.h"

class KTabListBox;

class ScoreDlg : public QDialog
{
    Q_OBJECT
public:
    ScoreDlg( QWidget *parent, const char *title, PlayerList *players );
    
private:
    void init( void );

    PlayerList *plrList;
    KTabListBox *scoreTable;

};

#endif // _SCOREDLG_H_


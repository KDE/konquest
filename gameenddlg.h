#ifndef _GAMEENDDLG_H_
#define _GAMEENDDLG_H_

#include <kdialog.h>
//Added by qt3to4:
#include <QLabel>

class QSlider;
class QPushButton;

class GameEndDlg : public KDialog
{
    Q_OBJECT

public:
    GameEndDlg( QWidget *parent );
    virtual ~GameEndDlg();
    
    int extraTurns();

private:
    void init();

private slots:
    void turnCountChange( int );
    void slotYes();
    
private:
    QSlider *turnCount;
    QLabel *turnCountLbl;
};

#endif // _GAMEENDDLG_H_


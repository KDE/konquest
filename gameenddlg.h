#ifndef _GAMEENDDLG_H_
#define _GAMEENDDLG_H_

#include <kdialogbase.h>

class QSlider;
class QPushButton;

class GameEndDlg : public KDialogBase
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


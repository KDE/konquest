#ifndef _GAMEENDDLG_H_
#define _GAMEENDDLG_H_

#include <qdialog.h>

class KSlider;
class QPushButton;

class GameEndDlg : public QDialog
{
    Q_OBJECT

public:
    GameEndDlg( QWidget *parent );
    virtual ~GameEndDlg();
    
    int addTurns;

private slots:
    void turnCountChange( int );
    
private:
    KSlider *turnCount;
    QLabel *turnCountLbl;
    
};

#endif // _GAMEENDDLG_H_


#ifndef _GAMEENDDLG_H_
#define _GAMEENDDLG_H_

#include <qdialog.h>

class QSlider;
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
    QSlider *turnCount;
    QLabel *turnCountLbl;
    
};

#endif // _GAMEENDDLG_H_


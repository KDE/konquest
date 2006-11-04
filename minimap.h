#ifndef _MINIMAP_H
#define _MINIMAP_H

#include <QWidget>

#include "gamecore.h"

class MiniMap : public QWidget
{
    Q_OBJECT

public:
    MiniMap( QWidget *parent = 0 );

    void setMap( Map *newMap );

protected:
    void paintEvent(QPaintEvent *event);
    
private:
    int SECTOR_SIZE;

    Map *map;
};


#endif // _MINIMAP_H_

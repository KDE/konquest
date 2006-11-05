#ifndef _MINIMAPVIEW_H
#define _MINIMAPVIEW_H

#include <QWidget>

#include "gamecore.h"

class MiniMapView : public QWidget
{
    Q_OBJECT

public:
    MiniMapView( QWidget *parent = 0 );

    void setMap( Map *newMap );

protected:
    void paintEvent(QPaintEvent *event);
    
private:
    int SECTOR_SIZE;

    Map *map;
};


#endif // _MINIMAP_H_
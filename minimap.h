#ifndef _MINIMAP_H
#define _MINIMAP_H

#include <qwidget.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qgridview.h>
#include <qtimer.h>

#include "gamecore.h"
#include "images.h"


class MiniMap : public QGridView
{
    Q_OBJECT

    // Constructors
public:
    MiniMap( QWidget *parent = 0, const char* name = 0 );
    virtual ~MiniMap();

    void setMap( Map *newMap );

protected:
    void paintCell( QPainter *p, int row, int col );

private slots:
    void mapUpdate();

private:
    int SECTOR_HEIGHT;
    int SECTOR_WIDTH;

    int BOARD_HEIGHT;
    int BOARD_WIDTH;

    void drawSector( QPainter *, Sector & );

    Map *map;
};


#endif // _MINIMAP_H_

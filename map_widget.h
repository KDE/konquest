#ifndef _MAP_WIDGET_H
#define _MAP_WIDGET_H


#include <qwidget.h>
#include <qframe.h>
#include <qpixmap.h>
#include <qtablevw.h>
#include <qtimer.h>

#include "gamecore.h"
#include "images.h"


class ConquestMap : public QTableView
{
    Q_OBJECT

    // Constructors
public:
    ConquestMap( Map *newMap, QWidget *parent = 0 );
    virtual ~ConquestMap();

    // Interface
public:
    void unselectPlanet( void );

protected:
    void mousePressEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
    void paintCell( QPainter *p, int row, int col );

private slots:
    void mapUpdate();
    void squareBlink();

signals:
    void planetSelected( Planet * );
    void planetHighlighted( Planet * );
    
private:
    const int SECTOR_HEIGHT;
    const int SECTOR_WIDTH;

    const int BOARD_HEIGHT;
    const int BOARD_WIDTH;

    void drawSector( QPainter *, Sector &, bool borderStrobe = true, bool highlight = false );

    Map *map;
    QColor gridColor;
    QFont labelFont;

    int hiLiteRow, hiLiteCol;
};


#endif

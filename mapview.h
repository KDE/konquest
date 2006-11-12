#ifndef _MAPVIEW_H
#define _MAPVIEW_H


#include <QFrame>
#include <KSvgRenderer>

#include "map.h"
#include "images.h"

class MapView : public QFrame
{
    Q_OBJECT

    // Constructors
public:
    explicit  MapView( Map *newMap, QWidget *parent = 0 );
    virtual  ~MapView();

    // Interface
public:
    void unselectPlanet();

private:
    virtual void  mousePressEvent( QMouseEvent *e );
    virtual void  mouseMoveEvent( QMouseEvent *e );

private slots:
    void  mapUpdate();
    void  squareBlink();

signals:
    void  planetSelected( Planet * );
    void  planetHighlighted( Planet * );

private:
    void drawSector( QPainter *, Sector * );
    void paintEvent( QPaintEvent* );
    void resizeEvent ( QResizeEvent * );
    
private:
    int  SECTOR_HEIGHT;
    int  SECTOR_WIDTH;

    int  BOARD_HEIGHT;
    int  BOARD_WIDTH;

    // The map we are showing. We are not the owner of this one.
    Map          *m_map;

    bool          m_blinkState;
    QTimer       *m_blinkTimer;
    QColor        m_gridColor;
    QFont         m_labelFont;
    Coordinate    m_hiLiteCoord;
    KSvgRenderer  m_planetRenderer;
};


#endif

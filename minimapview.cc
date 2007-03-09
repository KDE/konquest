#include <QPainter>
#include <QtDebug>

#include "minimapview.h"
#include "minimapview.moc"
#include "map.h"
#include "planet.h"
#include "player.h"


MiniMapView::MiniMapView( QWidget *parent )
  : QWidget( parent ),
    m_map( 0 )
{
    QPalette  pal = palette();
    pal.setColor( backgroundRole(), Qt::black );
    setPalette( pal );

    setMinimumSize( 100, 100 );
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MiniMapView::~MiniMapView()
{
}


void
MiniMapView::setMap(Map *map)
{
    m_map = map;
    connect( m_map, SIGNAL( update() ), this, SLOT( update() ) );
}


void MiniMapView::paintEvent(QPaintEvent * /*event*/)
{
    //TODO : OFFSETS AREN'T HANDLED CURRENTLY !

    QPainter painter(this);
    int sectorSize = height()/m_map->columns();
    if (width()/m_map->rows() < sectorSize)
        sectorSize = width()/m_map->rows() < sectorSize;
    int woffset = 0, hoffset = 0;
    /*woffset = (width() - m_map->rows()*sectorSize)/2;
    hoffset = (height() - m_map->columns()*sectorSize)/2;
    qDebug() << "OFFSETS :" << woffset << "x" << hoffset;
    qDebug() << "Widget size :" << width() << "x" << height();
    qDebug() << "Drawn  size :" << m_map->rows()*sectorSize << "x" << m_map->columns()*sectorSize;*/
    
    // Draw the black background
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(hoffset, woffset, m_map->rows()*sectorSize, m_map->columns()*sectorSize);

    // Now draw the planets...
    for (int col = 0 ; col < m_map->columns() ; col++) {
        for (int row = 0 ; row < m_map->rows() ; row++) {
            if (m_map->sector(QPoint(col, row))->planet() != 0) {
                painter.setBrush( m_map->sector(QPoint(col, row))
                        ->planet()->player()->color() );

                // Draw a circle in the planets color to show the planet.
                painter.drawEllipse( hoffset + row * sectorSize, woffset + col * sectorSize,
                        sectorSize, sectorSize);
            }
        }
    }
}

#include <QPainter>

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
    QPainter painter(this);
    float sectorSize = ((float)width())/m_map->columns();
    if (height()/m_map->rows() < sectorSize)
        sectorSize = ((float)height())/m_map->rows();
    float woffset = 0.0, hoffset = 0.0;
    woffset = ((float)width() - m_map->columns()*sectorSize)/2;
    hoffset = ((float)height() - m_map->rows()*sectorSize)/2;

    // Draw the black background
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(QRectF(woffset, hoffset, m_map->columns()*sectorSize, m_map->rows()*sectorSize));

    // Now draw the planets...
    for (int col = 0 ; col < m_map->columns() ; col++) {
        for (int row = 0 ; row < m_map->rows() ; row++) {
            if (m_map->sector(QPoint(col, row))->planet() != 0) {
                painter.setBrush( m_map->sector(QPoint(col, row))
                        ->planet()->player()->color() );

                // Draw a circle in the planets color to show the planet.
                painter.drawEllipse( QRectF(woffset + col * sectorSize, hoffset + row * sectorSize,
                        sectorSize, sectorSize));
            }
        }
    }
}

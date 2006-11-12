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


void MiniMapView::paintEvent(QPaintEvent */*event*/)
{
    // Non square map aren't handled currently...

    // Calculate the horizontal (width) offset, and the max usable
    // size for the map.
    int  size;
    int  sectorSize;
    int  woffset = 0;
    if (width() > height()) {
        woffset = ((width() - height())/2);
        size = height();
    } else {
        size = width();
    }
    sectorSize = size/(m_map->columns());
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(woffset, 0, size, size);
    
    for (int col = 0 ; col < m_map->columns() ; col++) {
        for (int row = 0 ; row < m_map->rows() ; row++) {
            if (m_map->sector(QPoint(col, row))->planet() != 0) {
                painter.setBrush( m_map->sector(QPoint(col, row))
				  ->planet()->player()->color() );

		// Draw a circle in the planets color to show the planet.
                painter.drawEllipse( woffset + row * sectorSize,
				     col * sectorSize,
				     sectorSize, sectorSize);
            }
        }
    }
}

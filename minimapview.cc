#include "minimapview.h"
#include "minimapview.moc"
#include "map.h"
#include "planet.h"
#include "player.h"
#include <QPainter>

MiniMapView::MiniMapView(  QWidget *parent)
    : QWidget( parent ),
    SECTOR_SIZE(12),
    map( 0 )
{
    QPalette pal = palette();
    pal.setColor( backgroundRole(), Qt::black );
    setPalette( pal );

    setMinimumSize( 100, 100 ); // FIX THIS ?
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void
MiniMapView::setMap(Map *newMap)
{
    map = newMap;
    connect( map, SIGNAL( update() ), this, SLOT( update() ) );
}

void MiniMapView::paintEvent(QPaintEvent */*event*/) {
    // Non square map aren't handled currently...
    // Calculate the horizontal (width) offset, and the max usable size for the map...
    int size;
    int woffset = 0;
    if (width() > height()) {
        woffset = ((width() - height())/2);
        size = height();
    } else {
        size = width();
    }
    SECTOR_SIZE = size/(map->getColumns());
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(woffset, 0, size, size);
    
    for (int c = 0 ; c < map->getColumns() ; c++) {
        for (int r = 0 ; r < map->getRows() ; r++) {
            if (map->getSector(QPoint(c,r)).getPlanet() != 0) {
                painter.setBrush( map->getSector(QPoint(c,r)).getPlanet()->player()->color() );
                painter.drawEllipse( woffset + r*SECTOR_SIZE, c*SECTOR_SIZE, SECTOR_SIZE, SECTOR_SIZE );
            }
        }
    }
}

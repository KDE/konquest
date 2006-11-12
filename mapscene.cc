#include "images.h"
#include "mapscene.moc"
#include "mapitems.h"
#include "map.h"
#include "sector.h"
#include <QGraphicsItem>
#include <QtDebug>

MapScene::MapScene (Map *map)
    : QGraphicsScene(),
    m_map(map)
{
    m_renderer = new KSvgRenderer(IMAGES_SVG);
    connect( m_map, SIGNAL( mapPopulated() ), this, SLOT( mapUpdate() ) );
}

void MapScene::mapUpdate() {
    QGraphicsItem *item;
    Sector *sector;
    
    while (items().count() > 0) {
        item = items()[0];
        removeItem(item);
        delete item;
    }
    int c = 0;
    addItem(new BackgroundItem(this));
    for (int i = 0 ; i < BOARD_ROWS ; i++) {
        for (int j = 0 ; j < BOARD_COLS ; j++) {
            sector = m_map->sector(Coordinate(i, j));
            if (sector->hasPlanet()) {
                c++;
                qDebug() << "Creating planet " << c;
                PlanetItem *item = new PlanetItem(this, sector);
                connect(item, SIGNAL(planetSelected (Planet *)), this, SLOT(planetItemSelected (Planet *)));
                addItem(item);
            }
        }
    }
}

void MapScene::planetItemSelected (Planet *planet) {
    emit planetSelected(planet);
}

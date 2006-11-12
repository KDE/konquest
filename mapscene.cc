#include "images.h"
#include "mapscene.moc"
#include "mapitems.h"
#include "map.h"
#include "sector.h"
#include <QGraphicsItem>
#include <QtDebug>

MapScene::MapScene (Map *map)
    : QGraphicsScene(),
    m_map(map),
    m_selectedPlanetItem(NULL),
    m_planetInfoItem(NULL)
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
    addItem(new BackgroundItem(this));
    for (int i = 0 ; i < BOARD_ROWS ; i++) {
        for (int j = 0 ; j < BOARD_COLS ; j++) {
            sector = m_map->sector(Coordinate(i, j));
            if (sector->hasPlanet()) {
                PlanetItem *item = new PlanetItem(this, sector);
                connect(item, SIGNAL(planetItemSelected (PlanetItem *)), this, SLOT(planetItemSelected (PlanetItem *)));
                item->setZValue(1.0);
                addItem(item);
            }
        }
    }
}

void MapScene::unselectPlanet() {
    if (m_selectedPlanetItem) {
        m_selectedPlanetItem->unselect();
    }
}

void MapScene::planetItemSelected (PlanetItem *item) {
    if (m_selectedPlanetItem)
        unselectPlanet();
    m_selectedPlanetItem = item;
    emit planetSelected(item->sector()->planet());
}

void MapScene::displayPlanetInfo (Planet *planet, QPoint pos) {
    if (!planet && m_planetInfoItem) {
        m_planetInfoItem->hide();
        return;
    }
    if (!m_planetInfoItem) {
        m_planetInfoItem = new PlanetInfoItem();
        addItem(m_planetInfoItem);
    }
    m_planetInfoItem->show();
    m_planetInfoItem->setPlanet(planet);
    m_planetInfoItem->setZValue(1.5);
    m_planetInfoItem->moveBy(pos.x()-m_planetInfoItem->x(), pos.y()-m_planetInfoItem->y());
    update();
}

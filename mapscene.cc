#include <QGraphicsItem>
#include <QPainter>
#include <QtDebug>

#include "map.h"
#include "sector.h"
#include "images.h"
#include "mapscene.moc"
#include "mapitems.h"


MapScene::MapScene (Map *map)
  : QGraphicsScene(),
    m_map(map),
    m_selectedPlanetItem(NULL),
    m_planetInfoItem(NULL)
{
    m_renderer = new KSvgRenderer(IMAGES_SVG);
    connect( m_map, SIGNAL( mapPopulated() ), this, SLOT( mapUpdate() ) );
}


void MapScene::mapUpdate()
{
    QGraphicsItem  *item;
    Sector         *sector;
    
    while (items().count() > 0) {
        item = items()[0];
        removeItem(item);
        delete item;
    }
    m_planetInfoItem = NULL;
    
    for (int i = 0 ; i < m_map->rows() ; i++) {
        for (int j = 0 ; j < m_map->columns() ; j++) {
            sector = m_map->sector(Coordinate(i, j));
            if (sector->hasPlanet()) {
                PlanetItem *item = new PlanetItem(this, sector);
                connect(item, SIGNAL(planetItemSelected (PlanetItem *)), 
                        this, SLOT(planetItemSelected (PlanetItem *)));
                item->setZValue(1.0);
                addItem(item);
            }
        }
    }
}


void MapScene::unselectPlanet()
{
    if (m_selectedPlanetItem) {
        m_selectedPlanetItem->unselect();
    }
}


void MapScene::planetItemSelected (PlanetItem *item)
{
    if (m_selectedPlanetItem)
        unselectPlanet();
    m_selectedPlanetItem = item;

    emit planetSelected(item->sector()->planet());
}

void MapScene::drawBackground ( QPainter * painter, const QRectF & /*rect*/ ) {
    qreal s_w = width()/m_map->columns();
    qreal s_h = height()/m_map->rows();
    m_renderer->render(painter, "background", QRectF(0, 0, m_map->columns()*s_w, m_map->rows()*s_h));
    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->setOpacity(0.5);
    for (int i = 0 ; i <= m_map->columns() ; i++) {
        painter->drawLine(QPointF(i*s_w, 0), QPointF(i*s_w, m_map->rows()*s_h));
    }
    for (int j = 0 ; j <= m_map->rows() ; j++) {
        painter->drawLine(QPointF(0, j*s_h), QPointF(m_map->columns()*s_w, j*s_h));
    }
}

void MapScene::displayPlanetInfo (Planet *planet)
{
    if (!planet && m_planetInfoItem) {
        m_planetInfoItem->hide();
        return;
    }
    
    if (planet) {
        QPointF pos(planet->sector()->coord().x() * width()/m_map->columns(),
                   planet->sector()->coord().y() * height()/m_map->rows());
        displayPlanetInfo(planet, pos);
    }
}
void MapScene::displayPlanetInfo (Planet *planet, const QPointF & pos)
{
    if (!planet) {
        if (m_planetInfoItem) 
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
    m_planetInfoItem->moveBy( pos.x()-m_planetInfoItem->x(),
                              pos.y()-m_planetInfoItem->y() );
    // Move to stay in the game field.
    if (pos.x() > width()/2) {
        m_planetInfoItem->moveBy(-m_planetInfoItem->boundingRect().width(), 0);
    }
    if (pos.y() > height()/2) {
        m_planetInfoItem->moveBy(0, -m_planetInfoItem->boundingRect().height());
    }
    update();
}

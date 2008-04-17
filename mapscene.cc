/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <QGraphicsItem>
#include <QPainter>
#include <kdebug.h>

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

void MapScene::selectPlanet(Planet *planet)
{
    foreach (PlanetItem *item, m_planetItems) {
        if (item->sector() == planet->sector()) {
            item->select();
            m_selectedPlanetItem = item;
            break;
        }
    }
}

void MapScene::mapUpdate()
{
    QGraphicsItem  *item;
    Sector         *sector;
    
    while (items().count() > 0) {
        item = items()[0];
        removeItem(item);
        m_planetItems.clear();
        delete item;
    }
    m_planetInfoItem = NULL;
    for (int i = 0 ; i < m_map->rows() ; i++) {
        for (int j = 0 ; j < m_map->columns() ; j++) {
            sector = m_map->sector(Coordinate(j, i));
            if (sector->hasPlanet()) {
                PlanetItem *item = new PlanetItem(this, sector);
                connect(item, SIGNAL(planetItemSelected (PlanetItem *)), 
                        this, SLOT(planetItemSelected (PlanetItem *)));
                item->setZValue(1.0);
                addItem(item);
                m_planetItems.append(item);
            }
        }
    }
}

void MapScene::unselectPlanet()
{
    if (m_selectedPlanetItem) {
        m_selectedPlanetItem->unselect();
        m_selectedPlanetItem = NULL;
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
    qreal m_sectorSize = getSectorSize();
    qreal m_horizontalOffset = itemsHorizontalOffset();

    qreal mapWidth = m_map->columns()*m_sectorSize;
    qreal mapHeight = m_map->rows()*m_sectorSize;

    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->fillRect(0, 0, width(), height(), Qt::black);
    m_renderer->render(painter, "background", QRectF(0, 0, width(), height()));
    m_renderer->render(painter, "screen", QRectF(m_horizontalOffset, -5, mapWidth, mapHeight + 5));
    painter->setOpacity(0.5);
    qreal lastLine = mapWidth + m_horizontalOffset;
    for (qreal i = m_horizontalOffset ; i <= lastLine ; i += m_sectorSize) {
        painter->drawLine(QPointF(i, 0), QPointF(i, mapHeight));
    }
    for (qreal j = 0 ; j <= mapHeight ; j += m_sectorSize) {
        painter->drawLine(QPointF(m_horizontalOffset, j), QPointF(mapWidth + m_horizontalOffset, j));
    }
}

void MapScene::displayPlanetInfo (Planet *planet)
{
    if (!planet && m_planetInfoItem) {
        m_planetInfoItem->hide();
        return;
    }
    
    if (planet) {
        QPointF pos(planet->sector()->coord().y() * getSectorSize() + itemsHorizontalOffset(),
                    planet->sector()->coord().x() * getSectorSize());
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

qreal MapScene::getSectorSize() {
    qreal s_w = width();
    s_w = s_w/m_map->columns();
    qreal s_h = height();
    s_h = s_h/m_map->rows();
    return qMin(s_w, s_h);
}

qreal MapScene::itemsHorizontalOffset() {
    return ((width() - m_map->columns()*getSectorSize())/2);
}

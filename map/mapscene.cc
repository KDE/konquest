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

#include "mapscene.h"

#include <QGraphicsItem>
#include <QPainter>
#include <kdebug.h>

#include "map.h"
#include "mapitems.h"
#include "../sector.h"
#include "../images.h"
#include "../game.h"
#include "../planet.h"

MapScene::MapScene (Game *game)
  : QGraphicsScene(),
    m_game(game),
    m_selectedPlanetItem(NULL),
    m_planetInfoItem(NULL),
    m_width(width()),
    m_height(height())
{
    m_renderer = new QSvgRenderer(IMAGES_SVG);
    m_pixmapCache = new KPixmapCache("konquest-pixmaps");
}

MapScene::~MapScene ()
{
    delete m_pixmapCache;
    delete m_renderer;
}

void MapScene::resizeScene(const QRectF& rect)
{
    m_width = rect.width();
    m_height = rect.height();

    setSceneRect(rect);
}

void MapScene::selectPlanet(Planet *planet)
{
    unselectPlanet();
    foreach (PlanetItem *item, m_planetItems) {
        if (item->sector() == planet->sector()) {
            item->select();
            m_selectedPlanetItem = item;
            break;
        }
    }
}

void MapScene::clearMap()
{
    QGraphicsItem  *item;
    while (items().count() > 0) {
        item = items()[0];
        removeItem(item);
        delete item;
    }
    m_planetItems.clear();
    m_planetInfoItem = NULL;
    m_selectedPlanetItem = NULL;
 }

void MapScene::mapUpdate()
{    
    Sector         *sector;

    clearMap();
    if(m_game->isRunning()) {
        for (int i = 0 ; i < map()->rows() ; i++) {
            for (int j = 0 ; j < map()->columns() ; j++) {
                sector = map()->sector(Coordinate(j, i));
                if (sector->hasPlanet()) {
                    PlanetItem *item = new PlanetItem(this, sector, m_game);
                    connect(item, SIGNAL(planetItemSelected (PlanetItem *)),
                            this, SLOT(planetItemSelected (PlanetItem *)));
                    item->setZValue(1.0);
                    addItem(item);
                    m_planetItems.append(item);
                }
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
    // NOTE: without this line, background is black when using Qt 4.6! Qt bug?
    painter->setCompositionMode( QPainter::CompositionMode_SourceOver );

    qreal m_sectorSize = getSectorSize();
    qreal m_horizontalOffset = itemsHorizontalOffset();

    qreal mapWidth = map()->columns()*m_sectorSize;
    qreal mapHeight = map()->rows()*m_sectorSize;

    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->fillRect(0, 0, m_width, m_height, Qt::black);
    m_renderer->render(painter, "background", QRectF(0, 0, m_width, m_height));
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
        m_planetInfoItem = new PlanetInfoItem(m_game);
        addItem(m_planetInfoItem);
    }

    m_planetInfoItem->show();
    m_planetInfoItem->setPlanet(planet);
    m_planetInfoItem->setZValue(1.5);
    m_planetInfoItem->moveBy( pos.x()-m_planetInfoItem->x(),
                              pos.y()-m_planetInfoItem->y() );
    // Move to stay in the game field.
    if (pos.x() > m_width/2) {
        m_planetInfoItem->moveBy(-m_planetInfoItem->boundingRect().width(), 0);
    }
    if (pos.y() > m_height/2) {
        m_planetInfoItem->moveBy(0, -m_planetInfoItem->boundingRect().height());
    }
    update();
}

qreal MapScene::getSectorSize() {
    qreal s_w = m_width;
    s_w = s_w/map()->columns();
    qreal s_h = m_height;
    s_h = s_h/map()->rows();
    return qMin(s_w, s_h);
}

qreal MapScene::itemsHorizontalOffset() {
    return ((m_width - map()->columns()*getSectorSize())/2);
}

/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mapscene.h"

#include <QGraphicsItem>
#include <QPainter>

#include "map.h"
#include "mapitems.h"
#include "../sector.h"
#include "../images.h"
#include "../game.h"
#include "../planet.h"


MapScene::MapScene (Game *game)
  : QGraphicsScene(game),
    m_game(game),
    m_selectedPlanetItem(nullptr),
    m_planetInfoItem(nullptr),
    m_width(width()),
    m_height(height())
{
    m_renderer = new QSvgRenderer(IMAGES_SVG);
    m_imageCache = new KImageCache(QStringLiteral("konquest-pixmaps"), 100000);
}


MapScene::~MapScene ()
{
    delete m_imageCache;
    delete m_renderer;
}


void
MapScene::resizeScene(const QRectF& rect)
{
    m_width = rect.width();
    m_height = rect.height();

    setSceneRect(rect);
}


void
MapScene::selectPlanet(Planet *planet)
{
    unselectPlanet();
    for (PlanetItem *item : qAsConst(m_planetItems)) {
        if (item->sector() == planet->sector()) {
            item->select();
            m_selectedPlanetItem = item;
            break;
        }
    }
}


void
MapScene::clearMap()
{
    QGraphicsItem  *item;
    while (items().count() > 0) {
        item = items()[0];
        removeItem(item);
        delete item;
    }
    m_planetItems.clear();
    m_planetInfoItem = nullptr;
    m_selectedPlanetItem = nullptr;
}


void
MapScene::mapUpdate()
{
    Sector *sector;

    clearMap();
    if(m_game->isRunning()) {
        for (int i = 0 ; i < map()->rows() ; i++) {
            for (int j = 0 ; j < map()->columns() ; j++) {
                sector = map()->sector(Coordinate(j, i));
                if (sector->hasPlanet()) {
                    PlanetItem *item = new PlanetItem(this, sector, m_game);
                    connect(item, &PlanetItem::planetItemSelected, this, &MapScene::planetItemSelected);
                    item->setZValue(1.0);
                    addItem(item);
                    m_planetItems.append(item);
                }
            }
        }
    }
}


void
MapScene::unselectPlanet()
{
    if (m_selectedPlanetItem) {
        m_selectedPlanetItem->unselect();
        m_selectedPlanetItem = nullptr;
    }
}


void
MapScene::planetItemSelected (PlanetItem *item)
{
    if (m_selectedPlanetItem)
        unselectPlanet();
    m_selectedPlanetItem = item;

    Q_EMIT planetSelected(item->sector()->planet());
}


void
MapScene::drawBackground ( QPainter * painter, const QRectF & /*rect*/ )
{
    qreal sectorSize = getSectorSize();
    qreal horizontalOffset = itemsHorizontalOffset();
    qreal verticalOffset = itemsVerticalOffset();

    qreal mapWidth = map()->columns()*sectorSize;
    qreal mapHeight = map()->rows()*sectorSize;

    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    painter->fillRect(0, 0, m_width, m_height, Qt::black);
    m_renderer->render(painter, QStringLiteral("background"), QRectF(0, 0, m_width, m_height));

    m_renderer->render(painter, QStringLiteral("screen"), QRectF(horizontalOffset, verticalOffset, mapWidth, mapHeight));
    painter->setOpacity(0.5);
    qreal lastLine = mapWidth + horizontalOffset;
    for (qreal i = horizontalOffset ; i <= lastLine ; i += sectorSize) {
        painter->drawLine(QPointF(i, verticalOffset), QPointF(i, mapHeight + verticalOffset));
    }
    lastLine = mapHeight + verticalOffset;
    for (qreal j = verticalOffset ; j <= lastLine ; j += sectorSize) {
        painter->drawLine(QPointF(horizontalOffset, j), QPointF(mapWidth + horizontalOffset, j));
    }
}


void
MapScene::displayPlanetInfo(Planet *planet)
{
    if (!planet) {
        if (m_planetInfoItem) {
            m_planetInfoItem->hide();
        }

        return;
    }

    /**
     * @todo Figure out why coord.y and coord.x seem to be used the other way
     * around.
     */

    QPointF pos(
        planet->sector()->coord().y() * getSectorSize() + itemsHorizontalOffset(),
        planet->sector()->coord().x() * getSectorSize() + itemsVerticalOffset()
    );

    displayPlanetInfo(planet, pos);
}


void
MapScene::displayPlanetInfo (Planet *planet, const QPointF &pos)
{
    if (!planet) {
        if (m_planetInfoItem) {
            m_planetInfoItem->hide();
        }

        return;
    }

    if (!m_planetInfoItem) {
        m_planetInfoItem = new PlanetInfoItem(m_game);
        addItem(m_planetInfoItem);
    }

    m_planetInfoItem->show();
    m_planetInfoItem->setPlanet(planet);
    m_planetInfoItem->setZValue(1.5);
    m_planetInfoItem->moveBy(
        pos.x() - m_planetInfoItem->x(),
        pos.y() - m_planetInfoItem->y()
    );

    // Move to stay in the game field.

    if (pos.x() > m_width / 2) {
        m_planetInfoItem->moveBy(-m_planetInfoItem->boundingRect().width(), 0);
    }

    if (pos.y() > m_height / 2) {
        m_planetInfoItem->moveBy(0, -m_planetInfoItem->boundingRect().height());
    }

    update();
}


qreal
MapScene::getSectorSize()
{
    qreal s_w = m_width / map()->columns();
    qreal s_h = m_height / map()->rows();

    return qMin(s_w, s_h);
}


qreal
MapScene::itemsHorizontalOffset()
{
    return (m_width - map()->columns() * getSectorSize()) / 2;
}


qreal
MapScene::itemsVerticalOffset()
{
    return (m_height - map()->rows() * getSectorSize()) / 2;
}

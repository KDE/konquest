/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>

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
#ifndef KONQUEST_MAPSCENE_H
#define KONQUEST_MAPSCENE_H


#include <QSvgRenderer>
#include <KPixmapCache>
#include <QGraphicsScene>
#include "mapitems.h"
#include "../game.h"

class Planet;
class PlanetItem;
class PlanetInfoItem;
class Map;


class MapScene: public QGraphicsScene
{
    Q_OBJECT

    public:
        explicit MapScene(Game *game);
        ~MapScene();

        QSvgRenderer  *renderer() const  { return m_renderer; }
        KPixmapCache  *pixmapCache() const  { return m_pixmapCache; }
        Map           *map()      const  { return m_game->map(); }

        void           selectPlanet(Planet *planet);
        void           unselectPlanet();
        void           displayPlanetInfo(Planet *planet, const QPointF & pos);
        void           displayPlanetInfo(Planet *planet);

        void           drawBackground( QPainter * painter, const QRectF & rect );

        qreal          itemsHorizontalOffset();
        qreal          getSectorSize();

        void           resizeScene(const QRectF& rect);

        void           mapUpdate();

    signals:
        void  planetSelected( Planet * );

    private slots:
        void  planetItemSelected (PlanetItem *);

    private:
        Game            *m_game;

        QSvgRenderer    *m_renderer;
        KPixmapCache    *m_pixmapCache;
        PlanetItem      *m_selectedPlanetItem;
        PlanetInfoItem  *m_planetInfoItem;
        QVector<PlanetItem*> m_planetItems;

        qreal m_width;
        qreal m_height;
};

#endif // KONQUEST_MAPSCENE_H

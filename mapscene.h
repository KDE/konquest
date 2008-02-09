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
#ifndef _MAPSCENE_H
#define _MAPSCENE_H


#include <KSvgRenderer>

#include <QGraphicsScene>
#include "mapitems.h"

class Planet;
class PlanetItem;
class PlanetInfoItem;
class Map;


class MapScene: public QGraphicsScene
{
    Q_OBJECT

    public:
        explicit MapScene(Map *map);
        ~MapScene() {}

        KSvgRenderer  *renderer() const  { return m_renderer; }
        Map           *map()      const  { return m_map; }

        void           selectPlanet(Planet *planet);
        void           unselectPlanet();
        void           displayPlanetInfo(Planet *planet, const QPointF & pos);
        void           displayPlanetInfo(Planet *planet);
        
        void           drawBackground( QPainter * painter, const QRectF & rect );
        
        qreal          itemsHorizontalOffset();
        qreal          getSectorSize();
    signals:
        void  planetSelected( Planet * );
        
    private slots:
        void  mapUpdate();
        void  planetItemSelected (PlanetItem *);
        
    private:
        Map             *m_map;
        KSvgRenderer    *m_renderer;
        PlanetItem      *m_selectedPlanetItem;
        PlanetInfoItem  *m_planetInfoItem;
        QVector<PlanetItem*> m_planetItems;
};

#endif

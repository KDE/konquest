/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

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
#include <QGraphicsScene>
#include <KImageCache>
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
        KImageCache   *imageCache() const  { return m_imageCache; }
        Map           *map()      const  { return m_game->map(); }

        void           selectPlanet(Planet *planet);
        void           unselectPlanet();

    /**
     * Show planet information box.
     *
     * @param planet
     * planet for which to show planet information or NULL to hide possibly
     * shown planet information
     */

    void displayPlanetInfo(Planet *planet);

        void           drawBackground( QPainter * painter, const QRectF & rect );


    /**
     * Get size of a sector in pixel.
     *
     * A sector is rendered as a square, so width and height are the same. The
     * size depends on the number of sectors per direction (horizontal and
     * vertical) as well as the dimensions of the board canvas.
     */

    qreal getSectorSize();

    /**
     * Get the horizontal offset.
     *
     * In case the board is tall, it is horizontally centered on the board
     * canvas.
     */

    qreal itemsHorizontalOffset();

    /**
     * Get the vertical offset.
     *
     * In case the board is wide, it is vertically centered on the board canvas.
     */

    qreal itemsVerticalOffset();

        void           resizeScene(const QRectF& rect);

        void           clearMap();
        void           mapUpdate();

    signals:
        void  planetSelected( Planet * );

    private slots:
        void  planetItemSelected (PlanetItem *);

private:

    /**
     * Show planet information box.
     *
     * @param planet
     * planet for which to show planet information or NULL to hide possibly
     * shown planet information
     *
     * @param pos
     * position relative to upper left edge of the board
     */

    void displayPlanetInfo(Planet *planet, const QPointF &pos);

        Game            *m_game;

        QSvgRenderer    *m_renderer;
        KImageCache     *m_imageCache;
        PlanetItem      *m_selectedPlanetItem;
        PlanetInfoItem  *m_planetInfoItem;
        QVector<PlanetItem*> m_planetItems;

        qreal m_width;
        qreal m_height;
};

#endif // KONQUEST_MAPSCENE_H

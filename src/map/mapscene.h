/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_MAPSCENE_H
#define KONQUEST_MAPSCENE_H


#include <QGraphicsScene>
#include <QSvgRenderer>

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
        ~MapScene() override;

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

        void           drawBackground( QPainter * painter, const QRectF & rect ) override;


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

    Q_SIGNALS:
        void  planetSelected( Planet * );

    private:
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

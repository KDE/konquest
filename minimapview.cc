/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>
    Copyright Sean D'Epagnier <geckosenator@gmail.com>

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
#include "minimapview.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

#include "planet.h"
#include "players/player.h"


MiniMapView::MiniMapView( QWidget *parent )
  : QWidget( parent ),
    m_map( 0 ), m_selection(-1, -1)
{
    QPalette  pal = palette();
    pal.setColor( backgroundRole(), Qt::black );
    setPalette( pal );

    setMinimumSize( 100, 100 );
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MiniMapView::~MiniMapView()
{
}


void
MiniMapView::setMap(Map *map)
{
    m_map = map;
    connect( m_map, SIGNAL( update() ), this, SLOT( update() ) );
}

void MiniMapView::CalculateOffsets (float &sectorSize, float &woffset, float &hoffset)
{
    sectorSize = ((float)width())/m_map->columns();
    if (height()/m_map->rows() < sectorSize)
        sectorSize = ((float)height())/m_map->rows();

    woffset = ((float)width() - m_map->columns()*sectorSize)/2;
    hoffset = ((float)height() - m_map->rows()*sectorSize)/2;
}

void MiniMapView::mousePressEvent ( QMouseEvent * event )
{
    float sectorSize, woffset, hoffset;
    CalculateOffsets(sectorSize, woffset, hoffset);

    m_selection = Coordinate((event->x() - woffset) / sectorSize,
                             (event->y() - hoffset) / sectorSize);

    const Sector *sector = m_map->sector(m_selection);
    if (sector && !sector->hasPlanet())
        m_selection = Coordinate(-1, -1);

    emit sectorSelected(m_selection);
}

void MiniMapView::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    float sectorSize, woffset, hoffset;
    CalculateOffsets(sectorSize, woffset, hoffset);

    // Draw the black background
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(QRectF(woffset, hoffset, m_map->columns() * sectorSize, m_map->rows() * sectorSize));

    // Draw selection
    if (hasSelection()) {
        painter.setBrush(Qt::cyan);
        painter.drawRect(QRectF(
            woffset + m_selection.x() * sectorSize,
            hoffset + m_selection.y() * sectorSize,
            sectorSize,
            sectorSize
        ));
    }

    // Now draw the planets...
    for (int col = 0 ; col < m_map->columns() ; col++) {
        for (int row = 0 ; row < m_map->rows() ; row++) {
            QPoint pt(col, row);
            Planet *planet = m_map->sector(pt)->planet();
            if (planet) {
                Player *player = planet->player();
                if (player) {
                    painter.setBrush(player->color());

                    // Draw a circle in the planets color to show the planet.
                    painter.drawEllipse(
                                QRectF(woffset + col * sectorSize,
                                       hoffset + row * sectorSize,
                                       sectorSize,
                                       sectorSize));
                }
            }
        }
    }
}

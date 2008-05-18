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
#include "mapview.h"
#include "mapview.moc"
#include "mapscene.h"
#include <QGraphicsScene>
#include <QResizeEvent>

MapView::MapView( MapScene* scene, QWidget *parent )
    : QGraphicsView(scene, parent), m_scene(scene)
{
    setCacheMode( QGraphicsView::CacheBackground );
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMouseTracking(true);

    setMinimumSize( sizeHint() );
    resize( sizeHint() );
}

void MapView::resizeEvent( QResizeEvent* ev )
{
    m_scene->setSceneRect(QRectF(0, 0, ev->size().width(), ev->size().height()));
    m_scene->pixmapCache()->discard();
    QGraphicsView::resizeEvent(ev);
}

QSize MapView::sizeHint() const
{
    return QSize( (int)m_scene->width(), (int)m_scene->height() );
}

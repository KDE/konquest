/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mapview.h"
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
    m_scene->resizeScene(QRectF(0, 0, ev->size().width(), ev->size().height()));
    QGraphicsView::resizeEvent(ev);
}

QSize MapView::sizeHint() const
{
    return QSize( (int)m_scene->width(), (int)m_scene->height() );
}

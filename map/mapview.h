/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_MAPVIEW_H
#define KONQUEST_MAPVIEW_H

#include <QGraphicsView>

class MapScene;

class MapView : public QGraphicsView
{
public:
    explicit MapView( MapScene* scene, QWidget *parent = nullptr );
    
private:
    void resizeEvent( QResizeEvent* ) override;
    QSize sizeHint() const override;

    MapScene* m_scene;
};

#endif // KONQUEST_MAPVIEW_H

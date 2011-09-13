/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright <inge@lysator.liu.se>
    Copyright <pinaraf@gmail.com>
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
#ifndef KONQUEST_MINIMAPVIEW_H
#define KONQUEST_MINIMAPVIEW_H


#include <QWidget>

#include "map/map.h"


class MiniMapView : public QWidget
{
    Q_OBJECT

public:
    explicit MiniMapView( QWidget *parent = 0 );
    ~MiniMapView();

    void setMap( Map *newMap );

    Coordinate selection() const { return m_selection; }

    bool hasSelection() const { return m_selection != Coordinate(-1, -1); }

signals:
    void sectorSelected(const Coordinate &coord);

protected:
    void mousePressEvent( QMouseEvent * event );
    void paintEvent(QPaintEvent *event);
    
private:
    void CalculateOffsets (float &, float &, float &);

    Map  *m_map;
    Coordinate m_selection;
};


#endif // KONQUEST_MINIMAPVIEW_H

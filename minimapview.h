/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>
    Copyright Sean D'Epagnier <geckosenator@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_MINIMAPVIEW_H
#define KONQUEST_MINIMAPVIEW_H

#include <QWidget>

#include "map/map.h"


class MiniMapView : public QWidget
{
    Q_OBJECT

public:
    explicit MiniMapView(QWidget *parent = nullptr);
    ~MiniMapView();

    void setMap(Map *newMap);

    /**
     * @note Always use hasSelection() first to make sure that the returned
     * selection is valid.
     */

    Coordinate selection() const { return m_selection; }

    bool hasSelection() const { return (m_selection.x() >= 0) && (m_selection.x() < m_map->columns()) && (m_selection.y() >= 0) && (m_selection.y() < m_map->rows()); }

Q_SIGNALS:
    void sectorSelected(const Coordinate &coord);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void CalculateOffsets(float &, float &, float &);

    Map  *m_map;
    Coordinate m_selection;
};

#endif // KONQUEST_MINIMAPVIEW_H

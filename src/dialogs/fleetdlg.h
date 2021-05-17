/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef KONQUEST_FLEETDLG_H
#define KONQUEST_FLEETDLG_H

#include <QDialog>

#include "fleet.h"

class QTableWidget;


class FleetDlg : public QDialog
{
public: 
    FleetDlg( QWidget *parent,
              const AttackFleetList &fleets,
              const AttackFleetList &newFleets,
              const AttackFleetList &standingOrders );
    AttackFleetList *uncheckedFleets();

    QSize sizeHint() const override;

private:
    void update();
    void setupTable();

    AttackFleetList  m_newFleetList;
    AttackFleetList  m_standingOrders;
    AttackFleetList  m_fleetList;
    QTableWidget     *m_fleetTable;
};

#endif // KONQUEST_FLEETDLG_H

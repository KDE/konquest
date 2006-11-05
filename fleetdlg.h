#ifndef FLEETDLG_H
#define FLEETDLG_H

#include <QDialog>

#include "fleet.h"

class QTableWidget;

class FleetDlg : public QDialog
{
public: 
    FleetDlg( QWidget *parent, AttackFleetList *fleets );

private:
    void init();

    AttackFleetList *fleetList;
    QTableWidget *fleetTable;
};

#endif

#ifndef FLEETDLG_H
#define FLEETDLG_H

#include <KDialog>

#include "fleet.h"

class QTableWidget;

class FleetDlg : public KDialog
{
public: 
    FleetDlg( QWidget *parent, AttackFleetList *fleets );

private:
    void init();

    AttackFleetList  *m_fleetList;
    QTableWidget     *m_fleetTable;
};


#endif

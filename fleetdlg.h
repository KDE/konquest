#ifndef FLEETDLG_H
#define FLEETDLG_H

#include <qdialog.h>
#include "gamecore.h"

class QListView;

class FleetDlg : public QDialog  {
      Q_OBJECT

public: 
	FleetDlg( QWidget *parent, AttackFleetList *fleets );

private:
    void init( void );

    AttackFleetList *fleetList;
    QListView *fleetTable;
};

#endif

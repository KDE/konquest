#ifndef FLEETDLG_H
#define FLEETDLG_H

#include <klistview.h>

#include <qdialog.h>

#include "gamecore.h"

class FleetDlgListViewItem : public Q3ListViewItem
{
	public:
		FleetDlgListViewItem(Q3ListView *parent, QString s1, QString s2, QString s3, QString s4, QString s5);
		int compare(Q3ListViewItem *i, int col, bool) const;
};

class FleetDlg : public QDialog  {

public: 
	FleetDlg( QWidget *parent, AttackFleetList *fleets );

private:
    void init();

    AttackFleetList *fleetList;
    Q3ListView *fleetTable;
};

#endif

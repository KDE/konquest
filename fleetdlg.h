#ifndef FLEETDLG_H
#define FLEETDLG_H

#include <klistview.h>

#include <qdialog.h>

#include "gamecore.h"

class FleetDlgListViewItem : public QListViewItem
{
	public:
		FleetDlgListViewItem(QListView *parent, QString s1, QString s2, QString s3, QString s4, QString s5);
		int compare(QListViewItem *i, int col, bool) const;
};

class FleetDlg : public QDialog  {

public: 
	FleetDlg( QWidget *parent, AttackFleetList *fleets );

private:
    void init();

    AttackFleetList *fleetList;
    QListView *fleetTable;
};

#endif

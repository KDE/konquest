#ifndef _SCOREDLG_H_
#define _SCOREDLG_H_

#include <k3listview.h>

#include <qdialog.h>

#include "gamecore.h"

class ScoreDlgListViewItem : public Q3ListViewItem
{
	public:
		ScoreDlgListViewItem(Q3ListView *parent, QString s1, QString s2, QString s3, QString s4, QString s5, QString s6);
		int compare(Q3ListViewItem *i, int col, bool) const;
};

class ScoreDlg : public QDialog
{

public:
    ScoreDlg( QWidget *parent, const QString& title, PlayerList *players );

private:
    void init();

    PlayerList *plrList;
    Q3ListView *scoreTable;

};

#endif // _SCOREDLG_H_


#ifndef _SCOREDLG_H_
#define _SCOREDLG_H_

#include <klistview.h>

#include <qdialog.h>

#include "gamecore.h"

class ScoreDlgListViewItem : public QListViewItem
{
	public:
		ScoreDlgListViewItem(QListView *parent, QString s1, QString s2, QString s3, QString s4, QString s5, QString s6);
		int compare(QListViewItem *i, int col, bool) const;
};

class ScoreDlg : public QDialog
{
    Q_OBJECT
public:
    ScoreDlg( QWidget *parent, const QString& title, PlayerList *players );

private:
    void init( void );

    PlayerList *plrList;
    QListView *scoreTable;

};

#endif // _SCOREDLG_H_


/*
    Copyright 2003 Russell Steffen <rsteffen@bayarea.net>
    Copyright 2003 Stephan Zehetner <s.zehetner@nevox.org>
    Copyright 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright 2006 Inge Wallin <inge@lysator.liu.se>
    Copyright 2006 Pierre Ducroquet <pinaraf@gmail.com>

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

#include "scoredlg.h"

#include <klocale.h>
#include <kcomponentdata.h>
#include <KStandardGuiItem>
#include <kguiitem.h>

#include <QHeaderView>

ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, QList<Player *> players )
    : KDialog(parent)
{
    setObjectName( QLatin1String( "ScoreDlg" ) );
    setModal( true );
    setCaption(title);
    setButtons( KDialog::Ok );

    m_standingsWidget = new StandingsWidget(this, players);

    setMainWidget( m_standingsWidget );
    connect(this, &ScoreDlg::okClicked, this, &ScoreDlg::accept);
}


ScoreDlg::~ScoreDlg()
{
}

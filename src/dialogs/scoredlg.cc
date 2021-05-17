/*
    SPDX-FileCopyrightText: 2003 Russell Steffen <rsteffen@bayarea.net>
    SPDX-FileCopyrightText: 2003 Stephan Zehetner <s.zehetner@nevox.org>
    SPDX-FileCopyrightText: 2006 Dmitry Suzdalev <dimsuz@gmail.com>
    SPDX-FileCopyrightText: 2006 Inge Wallin <inge@lysator.liu.se>
    SPDX-FileCopyrightText: 2006 Pierre Ducroquet <pinaraf@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "scoredlg.h"

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

ScoreDlg::ScoreDlg( QWidget *parent, const QString& title, const QList<Player *> &players )
    : QDialog(parent)
{
    setObjectName( QStringLiteral( "ScoreDlg" ) );
    setModal( true );
    setWindowTitle(title);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    QWidget *mainWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    mainLayout->addWidget(mainWidget);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    okButton->setDefault(true);
    okButton->setShortcut(Qt::CTRL | Qt::Key_Return);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ScoreDlg::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ScoreDlg::reject);

    m_standingsWidget = new StandingsWidget(this, players);
    mainLayout->addWidget(m_standingsWidget );
    mainLayout->addWidget(buttonBox);
}


ScoreDlg::~ScoreDlg()
{
}

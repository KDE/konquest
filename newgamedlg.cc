/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>

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
#include "newgamedlg.h"
#include "newgamedlg.moc"

#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <KStandardGuiItem>
#include <kglobal.h>

#include <KLineEdit>
#include <KComboBox>
#include <QHeaderView>
#include <QItemDelegate>
#include <QDebug>
#include "player.h"
#include "map.h"


/*************************************************************************
 New Game Dialog Members
 ************************************************************************/

static const QColor PlayerColors[MAX_PLAYERS] = { 
    QColor( 130, 130, 255 ),
    Qt::yellow,
    Qt::red,
    Qt::green,
    Qt::white,
    Qt::cyan,
    Qt::magenta,
    QColor( 235, 153, 46 ),
    QColor( 106, 157, 104 ),
    QColor( 131, 153, 128) 
    };

class playersListModel : public QAbstractTableModel
{
    public:
        playersListModel(QObject *parent) : QAbstractTableModel(parent)
        {
        }

        int rowCount(const QModelIndex &index = QModelIndex()) const
        {
            Q_UNUSED(index);
            return m_players.count();
        }

        int columnCount(const QModelIndex&) const
        {
            return 2;
        }

        QVariant data(const QModelIndex &index, int role) const
        {
            if (index.isValid())
            {
                int row = index.row();
                int column = index.column();
                if (role == Qt::DecorationRole && column == 0)
                {
                    return PlayerColors[row];
                }
                else if (role == Qt::DisplayRole)
                {
                    if (column == 0)
                    {
                        return m_players.at(row).first;
                    }
                    else if (column == 1)
                    {
                        switch (m_players.at(row).second)
                        {
                            case Human: return i18nc("A human player", "Human"); break;
                            case ComputerWeak: return i18n("Computer Weak"); break;
                            case ComputerNormal: return i18n("Computer Normal"); break;
                            case ComputerHard: return i18n("Computer Hard"); break;
                        }
                    }
                }
            }
            return QVariant();
        }

        QVariant headerData(int section, Qt::Orientation orientation, int role) const
        {
            if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
            {
                if (section == 0) return i18nc("The player name", "Name");
                else if (section == 1) return i18n("Type");
            }
            return QVariant();
        }

        bool setData(const QModelIndex &index, const QVariant &value, int role)
        {
            bool result = false;
            if (index.isValid())
            {
                int row = index.row();
                int column = index.column();

                if (column == 0)
                {
                    m_players[row].first = value.toString();
                    result = true;
                }
                else if (column == 1)
                {
qDebug() << "Setting the player type.";
                    QString text = value.toString();
                    if (text == i18nc("A human player", "Human")) m_players[row].second = Human;
                    else if (text == i18n("Computer Weak")) m_players[row].second = ComputerWeak;
                    else if (text == i18n("Computer Normal")) m_players[row].second = ComputerNormal;
                    else if (text == i18n("Computer Hard")) m_players[row].second = ComputerHard;
                    result = true;
                }
            }

            if (result) emit dataChanged(index, index);

            return result;
        }

        void addPlayer()
        {
            int players = m_players.count();
            if (players < MAX_PLAYERS)
            {
                bool invalidName = true;
                int i = 1;
                QString name;
                while (invalidName) {
                    name = QString( i18nc("Default player name is \"player \" + player number", "Player %1", i) );
                    invalidName = false;
                    for (int j = 0 ; !invalidName && j < m_players.count(); j++)
                    {
                        invalidName = (m_players.at(j).first == name);
                    }
                    i++;
                }
                beginInsertRows(QModelIndex(), players, players);
                m_players << QPair<QString, Player>( name, Human );
                endInsertRows();
            }
        }

        Qt::ItemFlags flags(const QModelIndex &) const
        {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        }

        void removePlayer(int row)
        {
            if (row >= 0 && row < m_players.count())
            {
                beginRemoveRows(QModelIndex(), row, row);
                m_players.removeAt(row);
                endRemoveRows();
            }
        }

        bool hasHumans() const
        {
            bool humans = false;
            for (int i = 0 ; !humans && i < m_players.count() ; ++i)
            {
                humans = (m_players.at(i).second == Human);
            }
            return humans;
        }

    private:
        // TODO Move where it belongs
        enum Player { Human, ComputerWeak, ComputerNormal, ComputerHard };

        QList< QPair<QString, Player> > m_players;
};

class playersListDelegate : public QItemDelegate
{
	public:
		playersListDelegate(QObject *parent) : QItemDelegate(parent)
		{
		}
		
		QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
		{
			if (index.column() == 0)
				return new KLineEdit(parent);
			else
				return new KComboBox(parent);
		}
		
		void setEditorData(QWidget *editor, const QModelIndex &index) const
		{
			if (index.column() != 0) {
				KComboBox *cbox = static_cast<KComboBox*>(editor);
				cbox->addItem(i18nc("A human player", "Human"));
				cbox->addItem(i18n("Computer Weak"));
				cbox->addItem(i18n("Computer Normal"));
				cbox->addItem(i18n("Computer Hard"));
				
				cbox->setCurrentIndex( cbox->findText(index.data( Qt::DisplayRole).toString()) );
			} else {
				KLineEdit *lineEdit = static_cast<KLineEdit*>(editor);
				lineEdit->setText(index.data(Qt::DisplayRole).toString());
			}
		}
		
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
		{
			if (index.column() != 0) {
				KComboBox *cbox = static_cast<KComboBox*>(editor);
			
				model->setData(index, cbox->currentText(), Qt::EditRole);
			} else {
				KLineEdit *lineEdit = static_cast<KLineEdit*>(editor);
				
				model->setData(index, lineEdit->text(), Qt::EditRole);
			}
		}
};

NewGameDlg::NewGameDlg( QWidget *parent, Map *pmap, QList<Player *> *players,
                        Player *neutralPlayer, QList<Planet *> *planets )
    : KDialog( parent),
      m_map(pmap),
      m_players(players),
      m_planets(planets),
      m_neutral(neutralPlayer)
{
    setCaption(i18n("Start New Game"));
    setButtons(KDialog::Ok|KDialog::Cancel);
    setDefaultButton(KDialog::NoDefault);
    showButtonSeparator(true);

    m_w = new NewGameDlgUI(this);
    m_w->map->setMap( m_map );

    playersListModel *model = new playersListModel(this);

    m_w->playerList->setModel(model);
    m_w->playerList->setItemDelegate(new playersListDelegate(this));
    m_w->playerList->header()->setResizeMode(QHeaderView::Stretch);
    
    connect(m_w->neutralPlanetsSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->widthSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->heightSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->rejectMap, SIGNAL(clicked()), this, SLOT(slotNewMap()));
    connect(m_w->addPlayerButton, SIGNAL(clicked()), this, SLOT(slotAddPlayer()));
    connect(m_w->removePlayerButton, SIGNAL(clicked()), this, SLOT(slotRemovePlayer()));
    connect(model, SIGNAL(dataChanged (const QModelIndex &, const QModelIndex &)), this, SLOT(slotNewMap()));
    init();

    setMainWidget(m_w);
    slotNewMap();
}


void
NewGameDlg::init()
{
    KConfigGroup config = KGlobal::config()->group("Game");

    int nrOfPlayers = config.readEntry("NrOfPlayers",0);
    if (nrOfPlayers < 2)
       nrOfPlayers = 2;
    if (nrOfPlayers > MAX_PLAYERS)
       nrOfPlayers = MAX_PLAYERS;

    int nrOfPlanets = config.readEntry("NrOfPlanets", 3);
    int sizeWid = config.readEntry("SizeWidth", 10);
    int sizeHeight = config.readEntry("SizeHeight", 10);

    m_w->neutralPlanetsSB->setValue(nrOfPlanets);
    m_w->widthSB->setValue(sizeWid);
    m_w->heightSB->setValue(sizeHeight);
    
    // Restore player names
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    for(int i = 0; i < nrOfPlayers; ++i)
    {
       QString keyName = QString("Player_%1").arg(i);
       QString keyType = QString("PlayerType_%1").arg(i);
       
       QString playerName = config.readEntry(keyName,QString());
       QString playerType = config.readEntry(keyType,QString());
       
       model->addPlayer();
       if (!playerName.isEmpty()) model->setData(model->index(i, 0), playerName, Qt::EditRole);
       model->setData(model->index(i, 1), playerType, Qt::EditRole);
    }
}

void
NewGameDlg::slotAddPlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    model->addPlayer();

    slotNewMap();
    enableButtonOk(model->rowCount() > 0);
}

void
NewGameDlg::slotRemovePlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    model->removePlayer(m_w->playerList->currentIndex().row());

    slotNewMap();
    enableButtonOk(model->rowCount() > 0);
}

void
NewGameDlg::slotOk()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    bool hasHumans = model->hasHumans();

    if (!hasHumans) {
        KMessageBox::information(this,
              i18n("The game is much more fun when you add a human player!"));
        return;
    }

    KDialog::accept();
}

void
NewGameDlg::save()
{
    KConfigGroup config = KGlobal::config()->group("Game");
    
    config.writeEntry("NrOfPlanets", m_w->neutralPlanetsSB->value());
    config.writeEntry("SizeWidth", m_w->widthSB->value());
    config.writeEntry("SizeHeight", m_w->heightSB->value());

    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    config.writeEntry("NrOfPlayers", model->rowCount());
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QString  keyName        = QString("Player_%1").arg(i);
        QString  playerName = model->data(model->index(i, 0), Qt::DisplayRole).toString();
        QString  keyType = QString("PlayerType_%1").arg(i);
        QString  playerType = model->data(model->index(i, 1), Qt::DisplayRole).toString();

        config.writeEntry(keyName, playerName);
        config.writeEntry(keyType, playerType);
    }
    config.sync();
}

void
NewGameDlg::slotNewMap()
{
    // Clear map, player and planet lists
    m_map->clearMap();

    while (!m_planets->isEmpty())
        delete m_planets->takeFirst();

    m_map->resizeMap(m_w->heightSB->value(), m_w->widthSB->value());

    while (!m_players->isEmpty())
        delete m_players->takeFirst();

    // Make player list
    // Does the name already exist in the list
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    for (int i = 0; i < model->rowCount(); ++i) {
        QString  playerName = model->data(model->index(i, 0), Qt::DisplayRole).toString();
        QColor  color = model->data(model->index(i, 0), Qt::DecorationRole).value<QColor>();

        // TODO: This is a bit ugly, maybe a isAI in model will be better.
        bool ai = model->data( model->index(i, 1), Qt::DisplayRole).toString() != i18nc("A human player", "Human");
        
        // TODO: This is not going to work as changing names/player
        //       type in the table needs to change name in m_players 
        //       without changing the map.
        m_players->append( Player::createPlayer( m_map, playerName, color, i, ai ));
    }
    // make the planets
    m_map->populateMap( *m_players, m_neutral,
            m_w->neutralPlanetsSB->value(),
            *m_planets );
}

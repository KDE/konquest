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

#include "gamelogic.h"
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
   playersListModel(QObject *parent, 
                    QList<Player *> *players)
      : QAbstractTableModel(parent), m_players(players)
        {
        }

        int rowCount(const QModelIndex &index = QModelIndex()) const
        {
            Q_UNUSED(index);
            return m_players->count();
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
                    return m_players->at(row)->color();
                }
                else if (role == Qt::DisplayRole)
                {
                    if (column == 0)
                    {
                        return m_players->at(row)->name();
                    }
                    else if (column == 1)
                    {
                        switch (m_players->at(row)->getAiLevel())
                        {
                            case Player::Human: return i18nc("A human player", "Human"); break;
                            case Player::ComputerWeak: return i18n("Computer Weak"); break;
                            case Player::ComputerNormal: return i18n("Computer Normal"); break;
                            case Player::ComputerHard: return i18n("Computer Hard"); break;
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

        bool setData(const QModelIndex &index, const QVariant &value, int)
        {
            bool result = false;
            if (index.isValid())
            {
                int row = index.row();
                int column = index.column();

                if (column == 0)
                {
                    m_players->at(row)->setName(value.toString());
                    result = true;
                }
                else if (column == 1)
                {
                    QString text = value.toString();
                    if (text == i18nc("A human player", "Human"))
                        m_players->at(row)->setAiLevel(Player::Human);
                    else if (text == i18n("Computer Weak"))
                        m_players->at(row)->setAiLevel(Player::ComputerWeak);
                    else if (text == i18n("Computer Normal"))
                        m_players->at(row)->setAiLevel(Player::ComputerNormal);
                    else if (text == i18n("Computer Hard"))
                        m_players->at(row)->setAiLevel(Player::ComputerHard);
                    result = true;
                }
            }

            if (result) emit dataChanged(index, index);

            return result;
        }

        Player *addPlayer(Map *map)
        {
            Player *player = NULL;
            int players = m_players->count();
            if (players < MAX_PLAYERS)
            {
                bool invalidName = true;
                int i = 1;
                QString name;
                while (invalidName) {
                    name = QString( i18nc("Default player name is \"player \" + player number", "Player %1", i) );
                    invalidName = false;
                    for (int j = 0 ; !invalidName && j < m_players->count(); j++)
                    {
                        invalidName = (m_players->at(j)->name() == name);
                    }
                    i++;
                }
                beginInsertRows(QModelIndex(), players, players);

                player = new Player( map, name, PlayerColors[i-2], i-1, Player::Human);
                m_players->append(player);
        
                endInsertRows();
            }
            return player;
        }

        Qt::ItemFlags flags(const QModelIndex &) const
        {
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
        }

        Player *removePlayer(int row)
        {
            Player *player = NULL;
            if (row >= 0 && row < m_players->count())
            {
                beginRemoveRows(QModelIndex(), row, row);
                player = m_players->at(row);
                m_players->removeAt(row);
                endRemoveRows();
            }
            return player;
        }

        bool hasHumans() const
        {
            bool humans = false;
            for (int i = 0 ; !humans && i < m_players->count() ; ++i)
            {
                humans = (m_players->at(i)->getAiLevel() == Player::Human);
            }
            return humans;
        }

    private:

        QList<Player *> *m_players;
};

class playersListDelegate : public QItemDelegate
{
	public:
		playersListDelegate(QObject *parent) : QItemDelegate(parent)
		{
		}
		
		QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
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
                        Player *neutralPlayer, GameOptions &options)
    : KDialog( parent),
      m_map(pmap),
      m_players(players),
      m_neutral(neutralPlayer),
      m_options(options)
{
    setCaption(i18n("Start New Game"));
    setButtons(KDialog::Ok|KDialog::Cancel);
    setDefaultButton(KDialog::NoDefault);
    showButtonSeparator(true);

    m_w = new NewGameDlgUI(this);
    m_w->map->setMap( m_map );

    connect(m_map, SIGNAL(selectionChanged(Sector*)),
            this, SLOT(slotUpdateSelection(Sector*)));

    playersListModel *model = new playersListModel(this, m_players);

    m_w->playerList->setModel(model);
    m_w->playerList->setItemDelegate(new playersListDelegate(this));
    m_w->playerList->header()->setResizeMode(QHeaderView::Stretch);
    
    connect(m_w->neutralPlanetsSB, SIGNAL(valueChanged(int)), this, SLOT(slotUpdateNeutrals(int)));
    connect(m_w->widthSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->heightSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->randomizeMap, SIGNAL(clicked()), this, SLOT(slotNewMap()));
    connect(m_w->addPlayerButton, SIGNAL(clicked()), this, SLOT(slotAddPlayer()));
    connect(m_w->removePlayerButton, SIGNAL(clicked()), this, SLOT(slotRemovePlayer()));

    connect(m_w->OwnerCB, SIGNAL(currentIndexChanged(int)), this, SLOT(slotNewOwner(int)));
    connect(m_w->KillPercentageSB, SIGNAL(valueChanged(double)), this, SLOT(slotNewKillPercentage(double)));
    connect(m_w->ProductionSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewProduction(int)));

    init();

    setMainWidget(m_w);
    slotNewMap();
}

void
NewGameDlg::updateOwnerCB()
{
    m_w->OwnerCB->clear();

    m_w->OwnerCB->addItem("vacant");
    foreach(Player *player, *m_players)
        m_w->OwnerCB->addItem(player->name());
    m_w->OwnerCB->addItem("neutral");
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
    
    m_w->BlindMapCB->setCheckState(
       config.readEntry("BlindMap", false) ? Qt::Checked : Qt::Unchecked);
    m_w->CumulativeProductionCB->setCheckState(
       config.readEntry("CumulativeProduction", false) ? Qt::Checked : Qt::Unchecked);
    m_w->ProductionAfterConquereCB->setCheckState(
       config.readEntry("ProductionAfterConquere", true) ? Qt::Checked : Qt::Unchecked);
    m_w->NeutralsShowShipsCB->setCheckState(
       config.readEntry("NeutralsShowShips", false) ? Qt::Checked : Qt::Unchecked);
    m_w->NeutralsShowStatsCB->setCheckState(
       config.readEntry("NeutralsShowStats", false) ? Qt::Checked : Qt::Unchecked);
    m_w->NeutralsProductionSB->setValue(config.readEntry("NeutralsProduction", 1));

    // Restore player names
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    for(int i = 0; i < nrOfPlayers; ++i)
    {
       QString keyName = QString("Player_%1").arg(i);
       QString keyType = QString("PlayerType_%1").arg(i);
       
       QString playerName = config.readEntry(keyName,QString());
       QString playerType = config.readEntry(keyType,QString());
       
       model->addPlayer(m_map);
       if (!playerName.isEmpty()) model->setData(model->index(i, 0), playerName, Qt::EditRole);
       model->setData(model->index(i, 1), playerType, Qt::EditRole);
    }
   updateOwnerCB();
}

void
NewGameDlg::slotAddPlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());

    Player *player = model->addPlayer(m_map);
    if(player)
        m_map->addPlayerPlanetSomewhere(player);

    enableButtonOk(model->rowCount() > 1);

    updateOwnerCB();
}

void
NewGameDlg::slotRemovePlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());

    Player *player = model->removePlayer(m_w->playerList->currentIndex().row());
    if(player) {
        m_map->removePlayerPlanets(player);
        delete player;
    }

    enableButtonOk(model->rowCount() > 1);

    updateOwnerCB();
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
NewGameDlg::slotUpdateSelection(Sector *selected)
{
    bool planet = selected->hasPlanet();

    m_w->KillPercentageSB->setEnabled(planet);
    m_w->ProductionSB->setEnabled(planet);

    disconnect(m_w->OwnerCB, SIGNAL(currentIndexChanged(int)), this, SLOT(slotNewOwner(int)));

    if(planet) {
        Planet *p = selected->planet();
        int f = m_w->OwnerCB->findText(p->player()->name());
        if(f>=0)
            m_w->OwnerCB->setCurrentIndex(f);
        else
            m_w->OwnerCB->setCurrentIndex(m_w->OwnerCB->count()-1);


        m_w->KillPercentageSB->setValue(p->killPercentage());
        m_w->ProductionSB->setValue(p->production());
    } else
        m_w->OwnerCB->setCurrentIndex(0);

    connect(m_w->OwnerCB, SIGNAL(currentIndexChanged(int)), this, SLOT(slotNewOwner(int)));
}
void
NewGameDlg::slotNewOwner(int index)
{
    Sector *selected = m_map->sector(m_map->selectedSector());
    if(selected->hasPlanet())
        delete selected->planet();

    if(index > 0)
        m_map->addPlanet(selected,
                         index - 1 < m_players->length()
                         ? (*m_players)[index-1]
                         :  m_neutral,
                         m_w->ProductionSB->value(),
                         m_w->KillPercentageSB->value());

    m_w->KillPercentageSB->setEnabled(index);
    m_w->ProductionSB->setEnabled(index);
}

void
NewGameDlg::slotNewKillPercentage(double value)
{
    Sector *selected = m_map->sector(m_map->selectedSector());
    if(selected->hasPlanet())
        selected->planet()->setKillPercentage(value);
}

void
NewGameDlg::slotNewProduction(int value)
{
    Sector *selected = m_map->sector(m_map->selectedSector());
    if(selected->hasPlanet())
        selected->planet()->setProduction(value);
}

void
NewGameDlg::save()
{
    KConfigGroup config = KGlobal::config()->group("Game");
    
    config.writeEntry("NrOfPlanets", m_w->neutralPlanetsSB->value());
    config.writeEntry("SizeWidth", m_w->widthSB->value());
    config.writeEntry("SizeHeight", m_w->heightSB->value());

#define CONFIG_WRITE_ENTRY_CB(NAME) \
    config.writeEntry(#NAME, m_options.NAME \
                      = m_w->NAME##CB->checkState() == Qt::Checked);

    CONFIG_WRITE_ENTRY_CB(BlindMap);
    CONFIG_WRITE_ENTRY_CB(CumulativeProduction);
    CONFIG_WRITE_ENTRY_CB(ProductionAfterConquere);
    CONFIG_WRITE_ENTRY_CB(NeutralsShowShips);
    CONFIG_WRITE_ENTRY_CB(NeutralsShowStats);

    config.writeEntry("NeutralsProduction",
                      m_options.NeutralsProduction = m_w->NeutralsProductionSB->value());

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

    m_map->resizeMap(m_w->heightSB->value(), m_w->widthSB->value());

    // make sure we have space for neutrals
    int maxneutrals = m_w->heightSB->value()*m_w->widthSB->value() - m_players->count();
    if(m_w->neutralPlanetsSB->value() > maxneutrals) {
        disconnect(m_w->neutralPlanetsSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
        m_w->neutralPlanetsSB->setValue(maxneutrals);
        connect(m_w->neutralPlanetsSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    }

    // make the planets
    m_map->populateMap( *m_players, m_neutral, m_w->neutralPlanetsSB->value());

    if(m_map->hasSelectedSector())
        slotUpdateSelection(m_map->sector(m_map->selectedSector()));
}

void
NewGameDlg::slotUpdateNeutrals(int count)
{
   while(m_map->playerPlanetCount(m_neutral) < count
         && m_map->addNeutralPlanetSomewhere(m_neutral)) ;

   while(m_map->playerPlanetCount(m_neutral) > count
         && m_map->removePlayerPlanet(m_neutral)) ;

    if(m_map->hasSelectedSector())
       slotUpdateSelection(m_map->sector(m_map->selectedSector()));
}

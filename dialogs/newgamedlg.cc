/*
    Copyright Russell Steffen <rsteffen@bayarea.net>
    Copyright Stephan Zehetner <s.zehetner@nevox.org>
    Copyright Dmitry Suzdalev <dimsuz@gmail.com>
    Copyright Inge Wallin <inge@lysator.liu.se>
    Copyright Pierre Ducroquet <pinaraf@gmail.com>
    Copyright 2011 Jeffrey Kelling <overlordapophis@gmail.com>

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

#include "../players/computerplayer.h"
#include "../players/localplayer.h"
#include "../game.h"
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
#include <QLinkedList>
#include <QPair>

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
    typedef QPair<QColor, QString> PlayerId;
    QLinkedList<PlayerId> m_availablePlayerId;
public:
    playersListModel(QObject *parent, Game *game)
        : QAbstractTableModel(parent), m_game(game)
    {
        for(int a = 0; a < MAX_PLAYERS; ++a)
        {
            m_availablePlayerId.push_back(PlayerId(
                PlayerColors[a], QString( i18nc("Default player name is \"player \" + player number", "Player %1", a) )));
        }
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
            Player *player = m_players.at(row);
            if (role == Qt::DecorationRole && column == 0)
            {
                return player->color();
            }
            else if (role == Qt::DisplayRole)
            {
                if (column == 0)
                {
                    return player->name();
                }
                else if (column == 1)
                {
                    LocalPlayer *lPlayer = qobject_cast<LocalPlayer*>(player);
                    ComputerPlayer *cPlayer = qobject_cast<ComputerPlayer*>(player);
                    if (lPlayer)
                    {
                        return i18nc("A human player", "Human");
                    }
                    else
                    {
                        switch (cPlayer->aiLevel())
                        {
                        case ComputerPlayer::Weak: return i18n("Computer Weak"); break;
                        case ComputerPlayer::Normal: return i18n("Computer Normal"); break;
                        case ComputerPlayer::Hard: return i18n("Computer Hard"); break;
                        }
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

            Player *player = m_players.at(row);
            if (column == 0)
            {
                player->setName(value.toString());
                result = true;
            }
            else if (column == 1)
            {
                QString text = value.toString();
                ComputerPlayer *cPlayer = qobject_cast<ComputerPlayer*>(player);
                if (cPlayer && (text == i18n("Computer Weak")))
                    cPlayer->setAiLevel(ComputerPlayer::Weak);
                else if (cPlayer && (text == i18n("Computer Normal")))
                    cPlayer->setAiLevel(ComputerPlayer::Normal);
                else if (cPlayer && (text == i18n("Computer Hard")))
                    cPlayer->setAiLevel(ComputerPlayer::Hard);
                else if (cPlayer && (text == i18nc("A human player", "Human")))
                {
                    LocalPlayer *newPlayer = new LocalPlayer(player->game(), cPlayer->name(), cPlayer->color());
                    m_players[row] = newPlayer;
                    cPlayer->deleteLater();
                    player = newPlayer;
                    cPlayer = 0;
                }
                else if (text != i18nc("A human player", "Human"))
                {
                    ComputerPlayer::AiLevel lvl = ComputerPlayer::Weak;
                    if (text == i18n("Computer Weak"))
                        lvl = ComputerPlayer::Weak;
                    else if (text == i18n("Computer Normal"))
                        lvl = ComputerPlayer::Normal;
                    else if (text == i18n("Computer Hard"))
                        lvl = ComputerPlayer::Hard;
                    cPlayer = new ComputerPlayer(player->game(), player->name(), player->color(), lvl);
                    m_players[row] = cPlayer;
                    player->deleteLater();
                    player = cPlayer;
                }
                result = true;
            }
        }

        if (result) {
            m_game->setPlayers(m_players);
            emit dataChanged(index, index);
        }

        return result;
    }

    Player *addPlayer()
    {
        Player *player = NULL;
        int players = m_players.count();
        if (players < MAX_PLAYERS)
        {
            beginInsertRows(QModelIndex(), players, players);

            player = new LocalPlayer(m_game, m_availablePlayerId.front().second, m_availablePlayerId.front().first);
            m_availablePlayerId.pop_front();
            m_players.append(player);
            m_game->setPlayers(m_players);

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
        if (row >= 0 && row < m_players.count())
        {
            beginRemoveRows(QModelIndex(), row, row);
            player = m_players.at(row);
            m_availablePlayerId.push_back(PlayerId(player->color(), player->name()));
            m_players.removeAt(row);
            m_game->setPlayers(m_players);
            endRemoveRows();
        }
        return player;
    }

    bool hasHumans() const
    {
        foreach (Player *player, m_players)
        {
            if (!player->isAiPlayer())
                return true;
        }
        return false;
    }

private:
    Game *m_game;
    QList<Player *> m_players;
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

NewGameDlg::NewGameDlg( QWidget *parent, Game *game)
    : KDialog( parent),
      m_game(game)
{
    m_neutral = m_game->neutral();
    setCaption(i18n("Start New Game"));
    setButtons(KDialog::Ok|KDialog::Cancel);
    setDefaultButton(KDialog::NoDefault);
    showButtonSeparator(true);

    m_w = new NewGameDlgUI(this);
    m_w->map->setMap(m_game->map());

    connect(m_w->map, SIGNAL(sectorSelected(Coordinate)),
            this, SLOT(slotUpdateSelection(Coordinate)));

    playersListModel *model = new playersListModel(this, m_game);

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

    m_w->OwnerCB->addItem(i18n("vacant"));
    foreach(Player *player, m_game->players())
        m_w->OwnerCB->addItem(player->name());
    m_w->OwnerCB->addItem(i18n("neutral"));
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

        model->addPlayer();
        if (!playerName.isEmpty())
            model->setData(model->index(i, 0), playerName, Qt::EditRole);
        model->setData(model->index(i, 1), playerType, Qt::EditRole);
    }
    updateOwnerCB();
}

void
NewGameDlg::slotAddPlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());

    Player *player = model->addPlayer();
    if (player)
        m_game->map()->addPlayerPlanetSomewhere(player);

    enableButtonOk(model->rowCount() > 1);

    updateOwnerCB();
}

void
NewGameDlg::slotRemovePlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());

    Player *player = model->removePlayer(m_w->playerList->currentIndex().row());
    if(player) {
        m_game->map()->removePlayerPlanets(player);
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
NewGameDlg::slotUpdateSelection(const Coordinate &coord)
{
    Sector *selected = m_game->map()->sector(coord);
    if (!selected)
        return;
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
    if (!m_w->map->hasSelection())
        return;
    Sector *selected = m_game->map()->sector(m_w->map->selection());
    if(selected->hasPlanet())
        delete selected->planet();

    if(index > 0)
        m_game->map()->addPlanet(selected,
                                 index - 1 < m_game->players().count()
                                 ? m_game->players()[index-1]
                                 :  m_neutral,
                                 m_w->ProductionSB->value(),
                                 m_w->KillPercentageSB->value());

    m_w->KillPercentageSB->setEnabled(index);
    m_w->ProductionSB->setEnabled(index);
}

void
NewGameDlg::slotNewKillPercentage(double value)
{
    Sector *selected = m_game->map()->sector(m_w->map->selection());
    if(selected->hasPlanet())
        selected->planet()->setKillPercentage(value);
}

void
NewGameDlg::slotNewProduction(int value)
{
    Sector *selected = m_game->map()->sector(m_w->map->selection());
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
    config.writeEntry(#NAME, m_game->options().NAME \
    = m_w->NAME##CB->checkState() == Qt::Checked);

    CONFIG_WRITE_ENTRY_CB(BlindMap);
    CONFIG_WRITE_ENTRY_CB(CumulativeProduction);
    CONFIG_WRITE_ENTRY_CB(ProductionAfterConquere);
    CONFIG_WRITE_ENTRY_CB(NeutralsShowShips);
    CONFIG_WRITE_ENTRY_CB(NeutralsShowStats);

    config.writeEntry("NeutralsProduction",
                      m_game->options().NeutralsProduction = m_w->NeutralsProductionSB->value());

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
    m_game->map()->clearMap();

    m_game->map()->resizeMap(m_w->heightSB->value(), m_w->widthSB->value());

    // make sure we have space for neutrals
    int maxneutrals = m_w->heightSB->value()*m_w->widthSB->value() - m_game->players().count();
    if(m_w->neutralPlanetsSB->value() > maxneutrals) {
        disconnect(m_w->neutralPlanetsSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
        m_w->neutralPlanetsSB->setValue(maxneutrals);
        connect(m_w->neutralPlanetsSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    }

    // make the planets
    m_game->map()->populateMap(m_game->players(), m_neutral, m_w->neutralPlanetsSB->value());

    if(m_w->map->hasSelection())
        slotUpdateSelection(m_w->map->selection());
}

void
NewGameDlg::slotUpdateNeutrals(int count)
{
    while(m_game->map()->playerPlanetCount(m_neutral) < count
          && m_game->map()->addNeutralPlanetSomewhere(m_neutral)) ;

    while(m_game->map()->playerPlanetCount(m_neutral) > count
          && m_game->map()->removePlayerPlanet(m_neutral)) ;

    if(m_w->map->hasSelection())
        slotUpdateSelection(m_w->map->selection());
}

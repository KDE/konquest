
#include <kconfig.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kpushbutton.h>
#include <KStandardGuiItem>
#include <kglobal.h>

#include <QLineEdit>
#include <QComboBox>
#include <QHeaderView>
#include <QItemDelegate>

#include "newgamedlg.h"
#include "newgamedlg.moc"
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
                            case Human: return i18n("Human"); break;
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
                if (section == 0) return i18n("Name");
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
                	QString text = value.toString();
                	if (text == i18n("Human")) m_players[row].second = Human;
                	else if (text == i18n("Computer Weak")) m_players[row].second = ComputerWeak;
                	else if (text == i18n("Computer Normal")) m_players[row].second = ComputerNormal;
                	else if (text == i18n("Computer Hard")) m_players[row].second = ComputerHard;
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
                    name = QString( i18n("Player %1", i) );
                    invalidName = false;
                    for (int j = 0 ; !invalidName && j < m_players.count(); j++)
                    {
                        invalidName = (m_players.at(j).first == name);
                    }
                    i++;
                }
                beginInsertRows(QModelIndex(), players, players + 1);
                m_players << QPair<QString, Player>( name, ComputerWeak );
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
				return new QLineEdit(parent);
			else
				return new QComboBox(parent);
		}
		
		void setEditorData(QWidget *editor, const QModelIndex &index) const
		{
			if (index.column() != 0) {
				QComboBox *cbox = static_cast<QComboBox*>(editor);
				cbox->addItem(i18n("Human"));
				cbox->addItem(i18n("Computer Weak"));
				cbox->addItem(i18n("Computer Normal"));
				cbox->addItem(i18n("Computer Hard"));
				
				cbox->setCurrentIndex( cbox->findText(index.data( Qt::DisplayRole).toString()) );
			} else {
				QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
				lineEdit->setText(index.data(Qt::DisplayRole).toString());
			}
		}
		
		void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
		{
			if (index.column() != 0) {
				QComboBox *cbox = static_cast<QComboBox*>(editor);
			
				model->setData(index, cbox->currentText(), Qt::EditRole);
			} else {
				QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
				
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

    m_w->playerList->setModel(new playersListModel(this));
    m_w->playerList->setItemDelegate(new playersListDelegate(this));
    m_w->playerList->header()->setResizeMode(QHeaderView::Stretch);

    connect(m_w->neutralPlanetsSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->widthSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->heightSB, SIGNAL(valueChanged(int)), this, SLOT(slotNewMap()));
    connect(m_w->rejectMap, SIGNAL(clicked()), this, SLOT(slotNewMap()));
    connect(m_w->addPlayerButton, SIGNAL(clicked()), this, SLOT(slotAddPlayer()));
    connect(m_w->removePlayerButton, SIGNAL(clicked()), this, SLOT(slotRemovePlayer()));
    init();

    setMainWidget(m_w);
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
       
    m_w->neutralPlanetsSB->setValue(nrOfPlanets);
    
    // Restore player names
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    for(int i = 0; i < nrOfPlayers; ++i)
    {
       QString key = QString("Player_%1").arg(i);
       
       QString playerName = config.readEntry(key,QString());
       
       model->addPlayer();
       
       if (!playerName.isEmpty()) model->setData(model->index(i, 0), playerName, Qt::EditRole);
    }
}

void
NewGameDlg::slotAddPlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    model->addPlayer();

    slotNewMap();
}

void
NewGameDlg::slotRemovePlayer()
{
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    model->removePlayer(m_w->playerList->currentIndex().row());

    slotNewMap();
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

    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QString  key        = QString("Player_%1").arg(i);
        QString  playerName = model->data(model->index(i, 0), Qt::DisplayRole).toString();

        // TODO this is a bit ugly, maybe a isAI in model will be better
        bool ai = model->data(model->index(i, 1), Qt::DisplayRole).toString() != i18n("Human");
        if (ai) {
           if (config.hasKey(key))
              config.deleteEntry(key);
        }
        else {
           config.writeEntry(key, playerName);
        }
    }
    config.sync();
}

void
NewGameDlg::slotNewMap()
{
    // Clear map,, player and planet lists
    m_map->clearMap();

    while (!m_planets->isEmpty())
        delete m_planets->takeFirst();

    while (!m_players->isEmpty())
        delete m_players->takeFirst();

    // Make player list
    // Does the name already exist in the list
    playersListModel *model = static_cast<playersListModel*>(m_w->playerList->model());
    for (int i = 0; i < model->rowCount(); ++i)
    {
        QString  playerName = model->data(model->index(i, 0), Qt::DisplayRole).toString();
        QColor  color = model->data(model->index(i, 0), Qt::DecorationRole).value<QColor>();
        // TODO this is a bit ugly, maybe a isAI in model will be better
        bool ai = model->data(model->index(i, 1), Qt::DisplayRole).toString() != i18n("Human");
        
        // TODO This is not going to work as changing names/player type in the table
        // needs to change name in m_players without changing the map
        m_players->append( Player::createPlayer( m_map, playerName, color, i, ai ));
    }
    
    // make the planets
    m_map->populateMap( *m_players, m_neutral,
            m_w->neutralPlanetsSB->value(),
            *m_planets );
}


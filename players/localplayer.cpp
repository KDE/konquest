#include "localplayer.h"
#include <QDebug>

LocalPlayer::LocalPlayer(Game *game, const QString &newName, const QColor &color) :
    Player(game, newName, color)
{
}

bool LocalPlayer::isAiPlayer()
{
    return false;
}

void LocalPlayer::play()
{
    qDebug() << name() << "::play";
    emit(canPlay());
}

void LocalPlayer::done()
{
    qDebug() << name() << "::done";
    emit(donePlaying());
}

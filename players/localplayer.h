#ifndef LOCALPLAYER_H
#define LOCALPLAYER_H

#include "player.h"

class LocalPlayer : public Player
{
    Q_OBJECT
public:
    explicit LocalPlayer(Game *game, const QString &newName, const QColor &color);

    virtual bool isAiPlayer();

    virtual void play();

    void done();
signals:
    void canPlay();

public slots:

};

#endif // LOCALPLAYER_H

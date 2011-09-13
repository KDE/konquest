#ifndef NEUTRALPLAYER_H
#define NEUTRALPLAYER_H

#include "player.h"

class NeutralPlayer : public Player
{
    Q_OBJECT
public:
    explicit NeutralPlayer(Game *game);

    virtual bool isDead();
    
    virtual bool isNeutral();

protected:
    virtual void play();

signals:

public slots:

};

#endif // NEUTRALPLAYER_H

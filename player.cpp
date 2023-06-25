#include "global.h"
#include "player.h"

Player::Player()
{
    // característiques i estats
    type=PLAYER_TYPE_1;
    status=PLAYER_STATUS_LIVE;
    hunting=false;
    onPlay=true;
    onHerd=false;

    // aparença de jugador
    playerFace.pixmapPath = "../imatges/arrow-lgreen20.png";
    playerFace.pixmap=new QGraphicsPixmapItem(QPixmap(playerFace.pixmapPath));

    // posicionament i orientació
    compass.setPos(0, 0);
    compass.setSense(COMPASS_N);
};

Player::~Player()
{   // focus
    delete playerFace.hookItem;
    delete playerFace.hookBrush;
    delete playerFace.hookColor;
    delete playerFace.pixmap;
    delete playerFace.keepoutItem;
    delete playerFace.keepoutBrush;
    delete playerFace.keepoutColor;
}

void Player::setPos(int xpos, int ypos)
{
    compass.setXPos(xpos);
    compass.setYPos(ypos);

    if (playerFace.pixmap!=nullptr)
        playerFace.pixmap->setPos(xpos, ypos);

    if (playerFace.hookItem!=nullptr)
        playerFace.hookItem->setPos(xpos-(playerFace.hookRadius/2)+(PIXMAP_WIDTH/2),
                                      ypos-(playerFace.hookRadius/2)+(PIXMAP_HEIGHT/2));

    if (playerFace.keepoutItem!=nullptr)
        playerFace.keepoutItem->setPos(xpos-(playerFace.keepoutRadius/2)+(PIXMAP_WIDTH/2),
                                         ypos-(playerFace.keepoutRadius/2)+(PIXMAP_HEIGHT/2));

}

void Player::setHook()
{    // pincell
    playerFace.hookColor = new QColor(0xaa,0xff,0xff);
    playerFace.hookColor->setAlpha(96);
    playerFace.hookBrush = new QBrush(*playerFace.hookColor);
    // focus
    playerFace.hookRadius=PLAYER_HOOK_RADIUS;
    playerFace.hookStartAngle=PLAYER_HOOK_START;
    playerFace.hookSpanAngle=PLAYER_HOOK_SPAN;
    playerFace.hookItem = new QGraphicsEllipseItem(0, 0, playerFace.hookRadius, playerFace.hookRadius);
    playerFace.hookItem->setBrush(*playerFace.hookBrush);
    playerFace.hookItem->setStartAngle(PLAYER_HOOK_START*16); // clock wise!! 0 = East
    playerFace.hookItem->setSpanAngle(PLAYER_HOOK_SPAN*16);   // counterclock wise!!
}

void Player::setKeepout()
{    // pincell
    playerFace.keepoutColor = new QColor(0xff, 0xaa, 0x00);
    playerFace.keepoutColor->setAlpha(96);
    playerFace.keepoutBrush = new QBrush(*playerFace.keepoutColor);
    // focus
    playerFace.keepoutRadius=PLAYER_KEEPOUT_RADIUS;
    playerFace.keepoutItem = new QGraphicsEllipseItem(0, 0, playerFace.keepoutRadius, playerFace.keepoutRadius);
    playerFace.keepoutItem->setBrush(*playerFace.keepoutBrush);
}

void Player::alignTo(int newdir)
{
    compass.setSense((compass.getSense()+newdir)/2);
    compass.pointTo();
}


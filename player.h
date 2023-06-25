#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"
#include "compass.h"

// si belluga i interacciona directament
#define PLAYER_MODE_STATIC    0 // no es mou per l'escena
#define PLAYER_MODE_DYNAMIC   1 // es mou per l'escena

#define PLAYER_STATUS_DEAD    0 // és mort
#define PLAYER_STATUS_LIVE    1 // és viu

#define PLAYER_TYPE_1   1
#define PLAYER_NONE     -1

// accions i esdeveniments
#define PLAYER_ACTION_GO        0   // anar endavant
#define PLAYER_ACTION_TLEFT     1   // tombrar a l'esquerra
#define PLAYER_ACTION_TRIGHT    2   // tombar a la dreta
#define PLAYER_ACTION_REVERSE   3   // girar cua
#define PLAYER_ACTION_WAIT      4   // esperar-se
#define PLAYER_ACTIONS          5   // nombre d'accions

#define PLAYER_HOOK_RADIUS      300
#define PLAYER_KEEPOUT_RADIUS   40
#define PLAYER_HOOK_START       60
#define PLAYER_HOOK_SPAN        60

struct PlayerAspect
{   // imatge associada
QGraphicsPixmapItem* pixmap=nullptr;
QGraphicsEllipseItem* hookItem=nullptr;
QGraphicsEllipseItem* keepoutItem=nullptr;
QColor* hookColor=nullptr;
QBrush* hookBrush=nullptr;
QColor* keepoutColor=nullptr;
QBrush* keepoutBrush=nullptr;
int hookRadius=PLAYER_HOOK_RADIUS;
int hookStartAngle=0;
int hookSpanAngle=0;
int keepoutRadius=PLAYER_KEEPOUT_RADIUS;
QString pixmapPath="../imatges/arrow-lgreen20.png";
};

class Player 
{
protected:
// característiques i estats
int type;       // de quina mena és
int status;     // estat
bool hunting;   // si éstà caçant
bool onPlay;    // si hi és al joc o no
bool onHerd;    // si és part del ramat

// control de posició i orientació
Compass compass;
struct PlayerAspect playerFace;

public:

    Player();
    ~Player();
    // característiques i estats   
    int getType(){return type;};
    int getStatus(){return status;};
    bool isHunting(){return hunting;};
    void isHunting(bool ishunting){hunting=ishunting;};
    bool isOnPlay(){return onPlay;};

    // aparença
    void setPixmapPath(QString pixpath){playerFace.pixmapPath=pixpath;};
    QString getPixmapPath(){return playerFace.pixmapPath;};
    void setPixmap(QGraphicsPixmapItem* playerpixmap){playerFace.pixmap=playerpixmap;};
    QGraphicsPixmapItem* getPixmap(){return playerFace.pixmap;};

    // posicionament i orientació
    void setPos(int, int);
    void setXPos (int xpos){compass.setXPos(xpos);};
    int getXPos(){return compass.getXPos();};
    void setYPos (int ypos){compass.setYPos(ypos);};
    int getYPos(){return compass.getYPos();};
    void setCompass(int direction){compass.setSense(direction);};
    int getCompass(){return compass.getSense();};    void pointTo(){compass.pointTo();};
    int getCompassAngle(){return compass.getCompassAngle();};
    void alignTo(int);  // mira d'aliniar el seu vector de moviment amb la direcció donada
    void pointTo(int x, int y){compass.pointTo(x,y);};

    // ganxo i àrea de seguretat
    void setHook();
    void setKeepout();
    int getHookRadius(){return playerFace.hookRadius;};
    int getHookSpanAngle(){return playerFace.hookSpanAngle;};
    void setHookRadius(int hookrad){playerFace.hookRadius=hookrad;};
    void setKeepoutRadius(int keeprad){playerFace.keepoutRadius=keeprad;};
    int getKeepoutRadius(){return playerFace.keepoutRadius;};
    void setHookItem(QGraphicsEllipseItem* hookitem){playerFace.hookItem=hookitem;};
    void setKeepoutIem(QGraphicsEllipseItem* keepitem){playerFace.keepoutItem=keepitem;};
    QGraphicsEllipseItem* getHookItem(){return playerFace.hookItem;};
    QGraphicsEllipseItem* getKeepoutItem(){return playerFace.keepoutItem;};
};

#endif // PLAYER_H

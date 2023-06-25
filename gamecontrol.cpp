#include "global.h"
#include "gamecontrol.h"

GameController::GameController(QGraphicsScene* scene, GameView* view)
{
    // assignem l'escena i la vista heretades
    gameScene=scene;
    gameView=view;

    // connectem els events de teclat i un temporitzador a la classe
    connect(view, SIGNAL(keyPressEvent(QKeyEvent*)), this, SLOT(touchMe(QKeyEvent*)));
    gameTamTam = new QTimer();
    connect(gameTamTam, SIGNAL(timeout()), this, SLOT(onTamTam()));
}

void GameController::gSetInit ()
{
    gameScene->setSceneRect(0,0,VIEW_WIDTH,VIEW_HEIGHT);
    gameView->setScene(gameScene);
    // fixa les mides i bloca els marcs
    gameView->setFixedSize(VIEW_WIDTH,VIEW_HEIGHT);
     // elimina les vores de la finestra principal
    gameView->setFrameStyle(0);
    // defineix la mida de l'escena
    gameScene->setSceneRect(0,0,VIEW_WIDTH,VIEW_HEIGHT);

    gameView->setStyleSheet("background-color:#94bbd0;");
    gameView->setWindowTitle("player-hunting-test");
    // programa el temporitzador
    gameTamTam->setInterval(GAME_WORLD_TIMER_TIME);
}

void GameController::gSetTheatre ()
{
    // crea un QPixmap amb la imatge de fons
//    gameBackImage=new QPixmap(backPixmapPath);
    // crea un pincell utilitzant el QPixmap com a textura
//    gameBrush=new QBrush(*gameBackImage);
    // estableix el pincell (brush) com a fons de l'escena
//    gameView->setBackgroundBrush(*gameBrush);
    // afegim els jugadors
    gMakePlayers(PLAYER_TYPE_1);
}

void GameController::gMakePlayers(int playertype)
{
int counter;
Player* newplayer=nullptr;
QGraphicsPixmapItem* facepixmap;

    if (playertype==PLAYER_TYPE_1)
        for (counter=0; counter<INITIAL_PLAYERS; counter++)
        {   // creem el jugador
            newplayer=new Player();
            // creem i afegim la imatge principal
            facepixmap = new QGraphicsPixmapItem(QPixmap(newplayer->getPixmapPath()));
            newplayer->setPixmap(facepixmap);

            if (counter==0)
            {   // inicialitcem el 'ganxo'
                newplayer->setHook();
            }
            // inicialitzem el cercle 'de seguretat'
            newplayer->setKeepout();
            // i l'assignem
            playerAdd(newplayer);
        }
}

void GameController::onTamTam()
{

    // belluguem els jugadors
    playersMotion();
    sceneCollisions();
//    herdMotion();

    if(playersPool.size()<2)
        gameTamTam->stop();
}

void GameController::herdMotion()
{

}

void GameController::playersMotion()
{
int counter;

    for (counter=0; (unsigned long)counter < playersPool.size(); counter++)
    {   // movem els jugadors
        // forcem un moviment aleatori, per ara
        if (playersPool[counter]->isHunting() == true)
            playerMove(playersPool[counter], PLAYER_ACTION_GO);
        else
            playerMove(playersPool[counter], ((alNumGen(0,5,1) < 5)? PLAYER_ACTION_GO : alNumGen(0, 2, 1)));
    }
}

//---------------------
// belluga el jugador
//---------------------

bool GameController::playerMove (Player* player, int action)
{
bool retval=false;

    // si és un jugador vàlid
    if (player!=nullptr)
    {   // hi haurà moviment
        retval=true;
        // mirem cap a on assenyala la bruïxola
        switch (action)
        {   // i n'adjustem les noves coordinades
        case PLAYER_ACTION_GO:
            playerGo(player);
            break;
        case PLAYER_ACTION_REVERSE:
            playerGoReverse(player);
            break;
        case PLAYER_ACTION_TLEFT:
            playerGoLeft(player);
            break;
        case PLAYER_ACTION_TRIGHT:
            playerGoRight(player);
            break;
        case PLAYER_ACTION_WAIT:
            break;
        default:    // no sap on vol anar!
            retval=false;
        }
    }
    return retval;
}

//-----------------------
// el jugador va endavant
//-----------------------

void GameController::playerGo(Player* player)
{
int newXPos=0;
int newYPos=0;

    if (player==nullptr)
        return;

    // enregistrem la seva posicio
    newXPos=player->getXPos();
    newYPos=player->getYPos();

    switch(player->getCompass())
    {   // recalculem la posició segon la brúixola
    case COMPASS_N:
        newYPos-=PLAYER_XDRIFT;
        break;
    case COMPASS_NE:
        newXPos+=PLAYER_XDRIFT;
        newYPos-=PLAYER_YDRIFT;
        break;
    case COMPASS_E:
        newXPos+=PLAYER_XDRIFT;
        break;
    case COMPASS_SE:
        newXPos+=PLAYER_XDRIFT;
        newYPos+=PLAYER_YDRIFT;
        break;
    case COMPASS_S:
        newYPos+=PLAYER_YDRIFT;
        break;
    case COMPASS_SW:
        newXPos-=PLAYER_XDRIFT;
        newYPos+=PLAYER_YDRIFT;
        break;
    case COMPASS_W:
        newXPos-=PLAYER_XDRIFT;
        break;
    case COMPASS_NW:
        newXPos-=PLAYER_XDRIFT;
        newYPos-=PLAYER_YDRIFT;
        break;
    default:
        break;
    }

    // el desplacem al nou lloc?
    // si la nova posició és dins l'escena
    if ((newXPos>=0 && newXPos <= VIEW_WIDTH-PIXMAP_WIDTH)
        && (newYPos >= 0 && newYPos <= VIEW_HEIGHT-PIXMAP_HEIGHT))
        // posem al'hora les coordinades del jugador
        player->setPos(newXPos, newYPos);
    else    // si no, retruc!
        switch (player->getCompass())
        {
        case COMPASS_N:
        case COMPASS_E:
        case COMPASS_S:
        case COMPASS_W:
            playerGoReverse(player);
            break;
        case COMPASS_NE:
            if (newXPos > VIEW_WIDTH-PIXMAP_WIDTH)
                playerGoLeft(player);
            else
                playerGoRight(player);
            break;
        case COMPASS_SE:
            if (newXPos > VIEW_WIDTH-PIXMAP_WIDTH)
                playerGoRight(player);
            else
                playerGoLeft(player);
            break;
        case COMPASS_SW:
            if (newXPos < 0)
                playerGoLeft(player);
            else
                playerGoRight(player);
            break;
        case COMPASS_NW:
            if (newXPos < 0)
                playerGoRight(player);
            else
                playerGoLeft(player);
            break;
        }
}

//--------------------------------
// el jugador tomba a l'esquerra
//--------------------------------

void GameController::playerGoLeft(Player* player)
{
int leftDir;

    if (player!=nullptr)
    {   // un tomb a l'esquerra tanca l'angle
        leftDir=player->getCompass() - 1;
        (leftDir < 0)? player->setCompass(COMPASS_STEPS - 1) : player->setCompass(leftDir);
        player->pointTo();
        playerRotation(player);
    }
}

//----------------------
// el jugador gira cua
//----------------------

void GameController::playerGoReverse(Player* player)
{
int newDir;

    if (player!=nullptr)
    {
        newDir=player->getCompass() + COMPASS_STEPS/2;
        (newDir >= COMPASS_STEPS)? player->setCompass(newDir-COMPASS_STEPS) : player->setCompass(newDir);
        player->pointTo();
        playerRotation(player);
    }
}

//----------------------------
// el jugador tomba a la dreta
//----------------------------
void GameController::playerGoRight(Player* player)
{
int rightDir;

    if (player!=nullptr)
    {   // un tomb a la dreta obre l'angle
        rightDir=player->getCompass() + 1;
        (rightDir >= COMPASS_STEPS)? player->setCompass(0) : player->setCompass(rightDir);
        player->pointTo();
        playerRotation(player);
    }
}

//----------------------------------------
// afegueix el jugador i l'hi busca lloc
//----------------------------------------
// fa servir el vector de jugadors
bool GameController::playerAdd(Player* newplayer)
{
bool retval=false;

    if (newplayer!=nullptr)
    {   // afegim el jugador a la llista de jugadors
        playersPool.push_back(newplayer);
        // per raons 'estètiques' afegim primer els cercles
        if(newplayer->getHookItem()!=nullptr)
            gameScene->addItem(newplayer->getHookItem());
        if(newplayer->getKeepoutItem()!=nullptr)
            gameScene->addItem(newplayer->getKeepoutItem());
        // i ara el jugador
        gameScene->addItem(newplayer->getPixmap());
        // forcem una orientació aleatòria
        newplayer->setCompass(alNumGen(0, COMPASS_STEPS, 1));
        newplayer->pointTo();
        playerRotation(newplayer);

        do
        {   // el busquem un lloc a l'escena, repetim fins trobar-ne un de lliure
            newplayer->setPos(alNumGen(0,VIEW_WIDTH-PIXMAP_WIDTH,PLAYER_XDRIFT),alNumGen(0,VIEW_HEIGHT-PIXMAP_HEIGHT,PLAYER_YDRIFT));
            // mirem si el lloc és ple
            // si no n'ha trobat cap, sortim
        } while (playerCollision(newplayer) == true);
        retval=true;
    }
    return retval;
}

//-----------------------------------------------
// control del joc per teclat (per proves)
//-----------------------------------------------

void GameController::touchMe (QKeyEvent* keyEvent)
{
static Player* gamePlayer0=nullptr;
int playerAction=0;

    if(gamePlayer0 == nullptr)
    {
        gamePlayer0=playerFinder(PLAYER_TYPE_1);

        if (gamePlayer0 == nullptr)
            return;
    }

    switch (keyEvent->key())
    {
    case Qt::Key_Up:
        playerAction=PLAYER_ACTION_GO;
        break;
    case Qt::Key_Left:
        playerAction=PLAYER_ACTION_TLEFT;
        break;
    case Qt::Key_Down:
        playerAction=PLAYER_ACTION_REVERSE;
        break;
    case Qt::Key_Right:
        playerAction=PLAYER_ACTION_TRIGHT;
        break;
    case Qt::Key_Space: // arrenca/atura el temporitzador
//        gamePlayer0->setCompass(COMPASS_N);
//        gamePlayer0->pointToCompass();
        (gameTamTam->isActive()==true)? gameTamTam->stop(): gameTamTam->start();
    default:
        return;
    }
    playerMove(gamePlayer0, playerAction);
}

//-----------------------------------------------
// dibuixem el jugador
//-----------------------------------------------
// si rotation = true, és que cal girar-lo
// si rotation = false, és un desplaçament lineal
//-----------------------------------------------

void GameController::playerRotation(Player *player)
{
QGraphicsPixmapItem* pixmap;
QGraphicsEllipseItem* hookitem;

    if (player==nullptr || player->getPixmap() == nullptr)
        return;

    pixmap=player->getPixmap();
    // fixem el punt de gir fem la rotació
    pixmap->setTransformOriginPoint(pixmap->boundingRect().center());
    pixmap->setRotation(player->getCompassAngle());

    hookitem=player->getHookItem();

    if(hookitem!=nullptr)
    {   // fixem el punt de gir fem la rotació
        hookitem->setTransformOriginPoint(hookitem->rect().center());
        hookitem->setRotation(player->getCompassAngle());
    }
}

Player* GameController::playerFinder(int playerType)
{
int counter;
Player* foundPlayer=nullptr;

    for (counter=0; (unsigned long)counter<playersPool.size(); counter++)
        if (playersPool[counter]->getType()==playerType)
        {
            foundPlayer = playersPool[counter];
            break;
        }
    return foundPlayer;
}

Player* GameController::playerFinder(int xpos, int ypos)
{
int counter;
Player* foundPlayer=nullptr;

    for (counter=0; (unsigned long)counter<playersPool.size(); counter++)
    {
        if (playersPool[counter]->getXPos()==xpos && playersPool[counter]->getYPos()==ypos)
        {
            foundPlayer = playersPool[counter];
            break;
        }
        else if (playersPool[counter]->getHookItem()!= nullptr)
            counter++;
    }
    return foundPlayer;
}

int GameController::isPlayer(QGraphicsItem* graphitem)
{   // és jugador o una altre cosa?
int retval=PLAYER_NONE;
int counter;

    if (graphitem== nullptr)
        return retval;

    for(counter=0; (unsigned int)counter < playersPool.size(); counter++)
        if(playersPool[counter]->getPixmap()==graphitem)
        {
            retval=counter;
            break;
        }
    return retval;
}

bool GameController::playerCollision(Player* player)
{   // es validen els contactes del jugador
QList<QGraphicsItem*> playerslist;
int playeritems=0;
bool retval;

    if (player==nullptr)
        return true;
    // si té imatge té un item
    if (player->getPixmap()!=nullptr)
        playeritems++;
    // si té cercle de seguretat té un item+
    if (player->getKeepoutItem()!=nullptr)
        playeritems++;
    // si té cercle de captura té un item+
    if (player->getHookItem()!=nullptr)
        playeritems++;

    playerslist=gameScene->collidingItems(player->getKeepoutItem());
    retval=(playerslist.size() > playeritems-1)?true : false;
    return retval;
}

int GameController::playerToPursuit(Player* player)
{   // es validen els contactes del jugador 0
int counter;
int foundplayer=PLAYER_NONE;
QList<QGraphicsItem*> playerslist;

    if (player==nullptr)
        return foundplayer;

    playerslist=gameScene->collidingItems(player->getHookItem());

    for (counter=0; counter < playerslist.size(); counter++)
    {
        foundplayer=isPlayer(playerslist[counter]);
        if (foundplayer != PLAYER_NONE && playersPool[foundplayer]!=player)
            break;
    }
    return foundplayer;
}

int GameController::playerToHunt(Player* player)
{   // es validen els contactes del jugador 0
int counter;
int foundplayer=PLAYER_NONE;
QList<QGraphicsItem*> playerslist;

    if (player==nullptr)
        return foundplayer;

    playerslist=gameScene->collidingItems(player->getKeepoutItem());

    for (counter=0; counter < playerslist.size(); counter++)
    {
        foundplayer=isPlayer(playerslist[counter]);
        if (foundplayer != PLAYER_NONE && playersPool[foundplayer]!=player)
            break;
    }
    return foundplayer;
}

void GameController::sceneCollisions()
{   // es validen els contactes del jugador 0
int huntplayer=PLAYER_NONE;
int pursuitplayer=PLAYER_NONE;

    huntplayer=playerToHunt(playersPool[0]);

    if (huntplayer!=PLAYER_NONE)
    {   // si n'hi ha cap en contacte, ens el pelem!
        gameScene->removeItem(playersPool[huntplayer]->getKeepoutItem());
        gameScene->removeItem(playersPool[huntplayer]->getPixmap());
        playersPool.erase(playersPool.begin()+huntplayer);
    }

    pursuitplayer=playerToPursuit(playersPool[0]);

    if (pursuitplayer!=PLAYER_NONE)
    {   // si és davant el perseguim
        playersPool[0]->isHunting(true);
        playersPool[0]->pointTo(playersPool[pursuitplayer]->getXPos(), playersPool[pursuitplayer]->getYPos());
        playerRotation(playersPool[0]);
    }
    else
        playersPool[0]->isHunting(false);
}

//
//  GameScene.cpp
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#include "GameScene.h"

#include "GameOverScene.h"
#include "MainMenuScene.h"

#include "SimpleAudioEngine.h"
#include <cstdlib>

using namespace cocos2d;

CCScene* GameScene::scene(int gameMode)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::createWithGameMode(gameMode);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene* GameScene::createWithGameMode(int gameMode)
{
    GameScene* gameSceneLayer = new GameScene();
    if (gameSceneLayer && gameSceneLayer->initWithGameMode(gameMode))
    {
        gameSceneLayer->autorelease();
        return gameSceneLayer;
    }
    else
    {
        delete gameSceneLayer;
        gameSceneLayer = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool GameScene::initWithGameMode(int gameMode)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    thisGameMode = gameMode;
    
    CCLOG("Player %d Joined", NetworkEngine::getInstance()->playerNr);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 05 The first unfinished song for the Minecraft documentary.mp3", true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    world = new b2World(gravity);
    
    worldLayer = CCLayer::create();
    
    CCSprite *background = CCSprite::create("background.png");
    background->setAnchorPoint(CCPoint(0.5, 0.5));
    background->setPosition(CCPoint(0, 0));
    background->setScale(2);
    worldLayer->addChild(background);
    
    //planet shape definition
    b2CircleShape planetShape;
    planetShape.m_p.Set(0, 0);
    planetShape.m_radius = 450/32;
    
    //planet fixture definition
    b2FixtureDef planetFixture;
    planetFixture.density=10;
    planetFixture.friction=0;
    planetFixture.restitution=0;
    planetFixture.shape=&planetShape;
    
    //create planet
    planet = CCSprite::create("planet.png");
    planet->setPosition(CCPoint(0, 0));
    planet->setTag(6);
    worldLayer->addChild(planet);
    
    //body definition for planet
    b2BodyDef planetBodyDef;
    planetBodyDef.type= b2_staticBody;
    planetBodyDef.userData=planet;
    planetBodyDef.position.Set(planet->getPosition().x/32,planet->getPosition().y/32);
    
    planetBody = world->CreateBody(&planetBodyDef);
    planetBody->CreateFixture(&planetFixture);
    planetBody->SetGravityScale(10);
    
    //PowerUP tilemap
    tilemapSpawn1 = CCTMXTiledMap::create("powerUp_tilemap.tmx");
    tileMapSpawn1Layer = tilemapSpawn1->layerNamed("baseLayer");
    tilemapSpawn1->setAnchorPoint(CCPoint(0.5, 0.5));
    tilemapSpawn1->setPosition(CCPoint(-450, 450));
    worldLayer->addChild(tilemapSpawn1);
    
    tilemapSpawn2 = CCTMXTiledMap::create("powerUp_tilemap.tmx");
    tileMapSpawn2Layer = tilemapSpawn2->layerNamed("baseLayer");
    tilemapSpawn2->setAnchorPoint(CCPoint(0.5, 0.5));
    tilemapSpawn2->setPosition(CCPoint(450, -450));
    worldLayer->addChild(tilemapSpawn2);
    
    //wormHole shape definition
    b2CircleShape wormHoleShape;
    wormHoleShape.m_p.Set(0, 0);
    wormHoleShape.m_radius = 160/32;
    
    //wormHole fixture definition
    b2FixtureDef wormHoleFixture;
    wormHoleFixture.density=0.1;
    wormHoleFixture.friction=1;
    wormHoleFixture.restitution=0;
    wormHoleFixture.shape=&wormHoleShape;
    
    //create wormHole
    wormHole = CCSprite::create("wormHole.png");
    wormHole->setPosition(CCPoint(-990,990));
    wormHole->setTag(5);
    worldLayer->addChild(wormHole);
    
    //body definition for wormHole
    b2BodyDef wormHoleBodyDef;
    wormHoleBodyDef.type= b2_dynamicBody;
    wormHoleBodyDef.userData=wormHole;
    wormHoleBodyDef.position.Set(wormHole->getPosition().x/32,wormHole->getPosition().y/32);
    
    wormHoleBody = world->CreateBody(&wormHoleBodyDef);
    wormHoleBody->CreateFixture(&wormHoleFixture);
    
    b2DistanceJointDef jointDef;
    jointDef.Initialize(wormHoleBody, planetBody, wormHoleBody->GetWorldCenter(), planetBody->GetWorldCenter());
    jointDef.collideConnected = true;
    world->CreateJoint(&jointDef);
    
    //ship shape definition
    b2CircleShape shipShape;
    shipShape.m_p.Set(0, 0);
    shipShape.m_radius = 80/32;
    
    //ship fixture definition
    b2FixtureDef shipFixture;
    shipFixture.density=1.5;
    shipFixture.friction=0;
    shipFixture.restitution=1;
    shipFixture.shape=&shipShape;
    
    //create ship 1
    ship1 = CCSprite::create("ship_blue.png");
    ship1->setPosition(CCPoint(-990,-990));
    ship1->setTag(1);
    worldLayer->addChild(ship1);
    
    ship1flame = CCSprite::create("ship_flame.png");
    ship1flame->setPosition(CCPoint(75, -10));
    ship1flame->setVisible(false);
    ship1->addChild(ship1flame);
    
    ship1shield = CCSprite::create("ship_shield.png");
    ship1shield->setPosition(CCPoint(75, 85));
    ship1shield->setVisible(false);
    ship1->addChild(ship1shield);
    
    //body definition for ship 1
    b2BodyDef shipBodyDef1;
    shipBodyDef1.type= b2_dynamicBody;
    shipBodyDef1.userData=ship1;
    shipBodyDef1.position.Set(ship1->getPosition().x/32,ship1->getPosition().y/32);
    
    shipBody1 = world->CreateBody(&shipBodyDef1);
    shipBody1->CreateFixture(&shipFixture);
    shipBody1->SetAngularDamping(0);
    shipBody1->SetTransform(shipBody1->GetPosition(), CC_DEGREES_TO_RADIANS(-45));
    
    //create ship 2
    
    if (thisGameMode == 4)
    {
        ship2 = CCSprite::create("ship_green.png");
        ship2->setPosition(CCPoint(990, 990));
        ship2->setTag(2);
        worldLayer->addChild(ship2);
        
        ship2flame = CCSprite::create("ship_flame.png");
        ship2flame->setPosition(CCPoint(75, -10));
        ship2flame->setVisible(false);
        ship2->addChild(ship2flame);
        
        ship2shield = CCSprite::create("ship_shield.png");
        ship2shield->setPosition(CCPoint(75, 85));
        ship2shield->setVisible(false);
        ship2->addChild(ship2shield);
    }
    else if (thisGameMode == 2)
    {
        ship2 = CCSprite::create("ship_ai.png");
        ship2->setPosition(CCPoint(990, 990));
        ship2->setTag(2);
        worldLayer->addChild(ship2);
    }
    
    //body definition for ship 2
    b2BodyDef shipBodyDef;
    shipBodyDef.type= b2_dynamicBody;
    shipBodyDef.userData=ship2;
    shipBodyDef.position.Set(ship2->getPosition().x/32,ship2->getPosition().y/32);
    
    shipBody2 = world->CreateBody(&shipBodyDef);
    shipBody2->CreateFixture(&shipFixture);
    shipBody2->SetAngularDamping(0);
    shipBody2->SetTransform(shipBody2->GetPosition(), CC_DEGREES_TO_RADIANS(135));
    
    //Set default view to centre
    CCPoint viewPoint = ccpSub(CCPoint(visibleSize.width/2, visibleSize.height/2), CCPoint(1548, 1548));
    worldLayer->setPosition(viewPoint);
    
    this->addChild(worldLayer);
    
    
    hudLayer = CCLayer::create();
    
    //turret heat display
    CCSprite* heatCircleS = CCSprite::create("progressBar.png");
    heatProgress = CCProgressTimer::create(heatCircleS);
    heatProgress->setType(kCCProgressTimerTypeRadial);
    heatProgress->setPercentage(0);
    heatProgress->setPosition(visibleSize.width - 250, visibleSize.height/2);
    hudLayer->addChild(heatProgress);
    
    //fire button
    CCSprite* fireButtonS = CCSprite::create("fireButton.png");
    CCSprite* fireButtonDisS = CCSprite::create("disabledButton.png");
    
    fireButton = CCMenuItemSprite::create(fireButtonS, fireButtonS, fireButtonDisS, this, menu_selector(GameScene::fireButtonCall));
    CCMenu* menuFire = CCMenu::create(fireButton, NULL);
    menuFire->setPosition(CCPoint(visibleSize.width - 250, visibleSize.height/2));
    hudLayer->addChild(menuFire);
    
    CCSprite* turnButtonSL = CCSprite::create("turnLeft.png");
    CCSprite* turnButtonDisabledSL = CCSprite::create("disabledButton.png");
    
    //turn buttons
    turnLeftButton = CCMenuItemSprite::create(turnButtonSL, turnButtonSL, turnButtonDisabledSL, this, menu_selector(GameScene::turnLeftButtonCall));
    CCMenu* menuTurnLeft = CCMenu::create(turnLeftButton, NULL);
    
    menuTurnLeft->setPosition(CCPoint(250, visibleSize.height/2 + 200));
    hudLayer->addChild(menuTurnLeft);
    
    CCSprite* turnButtonSR = CCSprite::create("turnRight.png");
    CCSprite* turnButtonDisabledSR = CCSprite::create("disabledButton.png");
    
    turnRightButton = CCMenuItemSprite::create(turnButtonSR, turnButtonSR, turnButtonDisabledSR, this, menu_selector(GameScene::turnRightButtonCall));
    CCMenu* menuTurnRight = CCMenu::create(turnRightButton, NULL);
    
    menuTurnRight->setPosition(CCPoint(250, visibleSize.height/2 - 200));
    hudLayer->addChild(menuTurnRight);
    
    //overheat message
    overHeatLabel = CCLabelTTF::create("OVERHEAT", "Kenvector Future.ttf", 50);
    overHeatLabel->setPosition(CCPoint(visibleSize.width/2, 150));
    overHeatLabel->setColor(ccRED);
    overHeatLabel->setVisible(false);
    hudLayer->addChild(overHeatLabel);
    
    //switch role message
    switchRoleLabel = CCLabelTTF::create("SWITCH", "Kenvector Future.ttf", 60);
    switchRoleLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height-350));
    hudLayer->addChild(switchRoleLabel);

    //status ui
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("panel.png");
    panel->setContentSize(CCSize(1000, 150));
    panel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height-150));
    hudLayer->addChild(panel);
    
    CCLabelTTF* statusLabel = CCLabelTTF::create("STATUS", "Kenvector Future.ttf", 50);
    statusLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height-150));
    hudLayer->addChild(statusLabel);
    
    for (int n = 0; n < 5; n++)
    {
        CCSprite* squareShadow = CCSprite::create("square_shadow.png");
        squareShadow->setPosition(CCPoint(visibleSize.width/2 - 200 - n*50, visibleSize.height-150));
        hudLayer->addChild(squareShadow);
    }
    
    for (int n = 0; n < 5; n++)
    {
        CCSprite* squareShadow = CCSprite::create("square_shadow.png");
        squareShadow->setPosition(CCPoint(visibleSize.width/2 + 200 + n*50, visibleSize.height-150));
        hudLayer->addChild(squareShadow);
    }
    
    squareBlue1 = CCSprite::create("square_blue.png");
    squareBlue1->setPosition(CCPoint(visibleSize.width/2 - 200 - 0*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue1);
    
    squareBlue2 = CCSprite::create("square_blue.png");
    squareBlue2->setPosition(CCPoint(visibleSize.width/2 - 200 - 1*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue2);
    
    squareBlue3 = CCSprite::create("square_blue.png");
    squareBlue3->setPosition(CCPoint(visibleSize.width/2 - 200 - 2*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue3);
    
    squareBlue4 = CCSprite::create("square_blue.png");
    squareBlue4->setPosition(CCPoint(visibleSize.width/2 - 200 - 3*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue4);
    
    squareBlue5 = CCSprite::create("square_blue.png");
    squareBlue5->setPosition(CCPoint(visibleSize.width/2 - 200 - 4*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue5);
    
    statusBlue = CCTMXTiledMap::create("status_blue_tilemap.tmx");
    tileMapStatusBlueLayer = statusBlue->layerNamed("baseLayer");
    statusBlue->setAnchorPoint(CCPoint(0.5, 0.5));
    statusBlue->setPosition(CCPoint(150, visibleSize.height-150));
    hudLayer->addChild(statusBlue);
    
    squareGreen1 = CCSprite::create("square_green.png");
    squareGreen1->setPosition(CCPoint(visibleSize.width/2 + 200 + 0*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen1);
    
    squareGreen2 = CCSprite::create("square_green.png");
    squareGreen2->setPosition(CCPoint(visibleSize.width/2 + 200 + 1*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen2);
    
    squareGreen3 = CCSprite::create("square_green.png");
    squareGreen3->setPosition(CCPoint(visibleSize.width/2 + 200 + 2*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen3);
    
    squareGreen4 = CCSprite::create("square_green.png");
    squareGreen4->setPosition(CCPoint(visibleSize.width/2 + 200 + 3*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen4);
    
    squareGreen5 = CCSprite::create("square_green.png");
    squareGreen5->setPosition(CCPoint(visibleSize.width/2 + 200 + 4*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen5);
    
    statusGreen = CCTMXTiledMap::create("status_green_tilemap.tmx");
    tileMapStatusGreenLayer = statusGreen->layerNamed("baseLayer");
    statusGreen->setAnchorPoint(CCPoint(0.5, 0.5));
    statusGreen->setPosition(CCPoint(visibleSize.width-150, visibleSize.height-150));
    hudLayer->addChild(statusGreen);
    
    this->addChild(hudLayer);
    
    //waiting room layer
    loadingLayer = CCLayer::create();
    loadingLayer->setTouchPriority(1);
    
    CCSprite *loadingBackground = CCSprite::create("background.png");
    loadingBackground->setAnchorPoint(CCPoint(0.5, 0.5));
    loadingBackground->setScale(2);
    loadingBackground->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    loadingLayer->addChild(loadingBackground);
    
    CCLabelTTF* loading = CCLabelTTF::create("WAITING FOR OTHERS", "Kenvector Future.ttf", 80);
    loading->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    loadingLayer->addChild(loading);
    
    this->addChild(loadingLayer);
    
    
    //Contact Listener Init
    _contactListener = new ContactListener();
    world->SetContactListener(_contactListener);
    
    //variable init
    score1 = 5;
    score2 = 5;
    
    PlayerAllJoined = false;
    
    ship1ShieldBool = false;
    ship2ShieldBool = false;
    
    ship1DoubleDamageBool = false;
    ship2DoubleDamageBool = false;
    
    ship1InvertDirectionBool = false;
    ship2InvertDirectionBool = false;
    
    isGameOver = false;
    
    isRoleFlipped = false;
    
    heatAmount = 0;
    maxHeatAmount = 20;
    
    scheduleUpdate();
        
    return true;
}

//callback method that updates the game in loop
void GameScene::update(float delta)
{    
    NetworkEngine::getInstance()->run();
    
    switch (NetworkEngine::getInstance()->getState())
    {
        case STATE_ROOMFULL:
        {
            NetworkEngine::getInstance()->setLastInput(INPUT_EXIT);
            
            CCTransitionFade* pScene = CCTransitionFade::create(0.7,MainMenu::scene(), ccWHITE);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
            break;
        case STATE_CONNECTED:
        case STATE_LEFT:
            break;
        case STATE_DISCONNECTED:
            NetworkEngine::getInstance()->connect();
            break;
        case STATE_CONNECTING:
        case STATE_JOINING:
        case STATE_JOINED:
        case STATE_LEAVING:
        case STATE_DISCONNECTING:
        default:
            break;
    }
    
    //if all player joined
//    if (true)
    if (PlayerAllJoined)
    {
        //if from Host
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            if (this->loadingLayer->isVisible())
            {
                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                NetworkEngine::getInstance()->sendEvent(8, eventContent);
                removeLoading();
                
                CCDelayTime* delay = CCDelayTime::create(30);
                CCCallFunc* spawnRunes = CCCallFunc::create(this, callfunc_selector(GameScene::spawnRunes));
                CCCallFunc* toggleInvertRole = CCCallFunc::create(this, callfunc_selector(GameScene::toggleInvertRole));
                CCSequence* seq = CCSequence::create(toggleInvertRole, spawnRunes, delay, NULL);
                CCRepeatForever* seqLoop = CCRepeatForever::create(seq);
                this->runAction(seqLoop);
            }
        }
    }
    
    if (NetworkEngine::getInstance()->playerNr == 1)
    {
        sendPositions();
    }
    
    if (thisGameMode == 2)
    {
        moveSeekingAI();
    }
    
    //overheat gun mechanism
    heatAmount = heatAmount - delta*10/2;
    
    if (heatAmount > maxHeatAmount)
    {
        heatAmount = maxHeatAmount;
    }
    
    if (heatAmount < 0)
    {
        heatAmount = 0;
    }
    
    heatProgress->setPercentage(heatAmount/maxHeatAmount*100);
    
    //while there's an event from the network to be executed
    while (!NetworkEngine::getInstance()->eventQueue.empty())
    {
        std::vector<float> arr = NetworkEngine::getInstance()->eventQueue.front();
        NetworkEngine::getInstance()->eventQueue.pop();
        
        int code = arr.back();
        arr.pop_back();
        
        switch (code)
        {
            //Update position
            case 1:
            {
                float y3 = arr.back();
                arr.pop_back();
                
                float x3 = arr.back();
                arr.pop_back();
                
                float angle2 = arr.back();
                arr.pop_back();
                
                float y2 = arr.back();
                arr.pop_back();
                
                float x2 = arr.back();
                arr.pop_back();
                
                float angle1 = arr.back();
                arr.pop_back();
                
                float y1 = arr.back();
                arr.pop_back();
                
                float x1 = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                //If information is from Host (Player 1)
                if (playerNr == 1)
                {
                    //Correction of Ship 1 with dead reckoning
                    
                    b2Vec2 velocity = shipBody1->GetLinearVelocity();
                    int delay = NetworkEngine::getInstance()->getRoundTripTime()/100 / 2;
                    
                    if (delay < 3)
                    {
                        b2Vec2 futurePosition = b2Vec2(x1 + velocity.x * delay, y1 + velocity.y * delay);
                        float futureAngle = angle1 + shipBody1->GetAngularVelocity() * delay;
                        
                        shipBody1->SetTransform(futurePosition, futureAngle);
                    }
                    else
                    {
                        shipBody1->SetTransform(b2Vec2(x1, y1), angle1);
                    }
                    
                    //Correction of Ship 2 with dead reckoning
                    
                    velocity = shipBody2->GetLinearVelocity();
                    delay = NetworkEngine::getInstance()->getRoundTripTime()/100 / 2;
                    
                    if (delay < 3)
                    {
                        b2Vec2 futurePosition = b2Vec2(x2 + velocity.x * delay, y2 + velocity.y * delay);
                        float futureAngle = angle2 + shipBody2->GetAngularVelocity() * delay;
                        
                        shipBody2->SetTransform(futurePosition, futureAngle);
                    }
                    else
                    {
                        shipBody2->SetTransform(b2Vec2(x2, y2), angle2);
                    }
                    
                    //Correction of wormHole with dead reckoning
                    
                    velocity = wormHoleBody->GetLinearVelocity();
                    delay = NetworkEngine::getInstance()->getRoundTripTime()/100 / 2;
                    
                    if (delay < 3)
                    {
                        b2Vec2 futurePosition = b2Vec2(x3 + velocity.x * delay, y3 + velocity.y * delay);
                        float futureAngle = angle2 + wormHoleBody->GetAngularVelocity() * delay;
                        
                        wormHoleBody->SetTransform(futurePosition, futureAngle);
                    }
                    else
                    {
                        wormHoleBody->SetTransform(b2Vec2(x3, y3), wormHoleBody->GetAngle());
                    }
                }
                
                break;
            }
            //Shooting
            case 2:
            {
                float angle = arr.back();
                arr.pop_back();
                
                float y = arr.back();
                arr.pop_back();
                
                float x = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr != NetworkEngine::getInstance()->playerNr)
                {
                    this->shoot(playerNr);
                    
                    if (bulletBody->GetUserData() != NULL)
                    {
                        b2Vec2 velocity = bulletBody->GetLinearVelocity();
                        int delay = NetworkEngine::getInstance()->getRoundTripTime()/100 / 2;
                        
                        b2Vec2 futurePosition = b2Vec2(x + velocity.x * delay, y + velocity.y * delay);
                        
                        bulletBody->SetTransform(futurePosition, angle);
                    }
                }
                
                break;
            }
            //Someone got hit
            case 3:
            {
                int victim = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->someOneGotHit(victim);
                }
                break;
            }
            //Turn
            case 4:
            {
                int direction = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                this->turn(playerNr, direction);
                
                break;
            }
            //Shield
            case 5:
            {
                int ship = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->toggleShield(ship);
                }
                
                break;
            }
            //GameOver
            case 6:
            {
                int score2 = arr.back();
                arr.pop_back();
                
                int score1 = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->gameOver();
                }
                
                break;
            }
            //Player joined room
            case 7:
            {
                int players = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    if (players == 2)
                    {
                        if (thisGameMode == 2)
                        {
                            PlayerAllJoined = true;
                        }
                    }
                    else if (players == 4)
                    {
                        if (thisGameMode == 4)
                        {
                            PlayerAllJoined = true;
                        }
                    }
                }
                
                break;
            }
            //All player joined
            case 8:
            {
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->removeLoading();
                }
                
                break;
            }
            //HPUp
            case 9:
            {
                int ship = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->toggleHPUp(ship);
                }
                
                break;
            }
            //DoubleDamage
            case 10:
            {
                int ship = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->toggleDoubleDamage(ship);
                }
                
                break;
            }
            //Invert Direction
            case 11:
            {
                int ship = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->toggleInvertDirection(ship);
                }
                
                break;
            }
            //Spawn Rune
            case 12:
            {
                int rune2 = arr.back();
                arr.pop_back();
                
                int rune1 = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    tileMapSpawn2Layer->setTileGID(rune2, CCPoint(0, 0));
                    tileMapSpawn1Layer->setTileGID(rune1, CCPoint(0, 0));
                }
                
                break;
            }
            case 13:
            {
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->toggleInvertRole();
                }
                break;
            }
                
            default:
                break;
        }
        
        
    }
    
    //RuneSpawn1 Ship1
    if (ship1->boundingBox().intersectsRect(tilemapSpawn1->boundingBox()))
    {
        //Shield Rune
        if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 2 && !ship1ShieldBool)
        {
            tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleShield(1);
                CCLOG("Toggle shield 1");
            }
        }
        
        //HPup Rune
        else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 3)
        {
            tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleHPUp(1);
                CCLOG("Toggle Hp UP 1");
            }
        }
        
        //Double Damage Rune
        else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 5)
        {
            tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleDoubleDamage(1);
                CCLOG("Toggle DD 1");
            }
        }
        
        //Invert Direction Rune
        else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 4)
        {
            tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleInvertDirection(1);
                CCLOG("Toggle invert Direction 1");
            }
        }
    }
    
    //RuneSpawn2 Ship1
    if (ship1->boundingBox().intersectsRect(tilemapSpawn2->boundingBox()))
    {
        //Shield Rune
        if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 2 && !ship1ShieldBool)
        {
            tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleShield(1);
            }
        }
        
        //HPup Rune
        else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 3)
        {
            tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleHPUp(1);
            }
        }
        
        //Double Damage Rune
        else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 5)
        {
            tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleDoubleDamage(1);
            }
        }
        
        //Invert Direction Rune
        else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 4)
        {
            tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
            
            if (NetworkEngine::getInstance()->playerNr == 1)
            {
                toggleInvertDirection(1);
            }
        }
    }
    
    if (thisGameMode == 4)
    {
        //RuneSpawn1 Ship2
        if (ship2->boundingBox().intersectsRect(tilemapSpawn1->boundingBox()))
        {
            //Shield Rune
            if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 2 && !ship2ShieldBool)
            {
                tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleShield(2);
                }
            }
            
            //HPup Rune
            else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 3)
            {
                tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleHPUp(2);
                }
            }
            
            //Double Damage Rune
            else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 5)
            {
                tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleDoubleDamage(2);
                }
            }
            
            //Invert Direction Rune
            else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 4)
            {
                tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleInvertDirection(2);
                }
            }
        }
        
        //RuneSpawn2 Ship2
        if (ship2->boundingBox().intersectsRect(tilemapSpawn2->boundingBox()))
        {
            //Shield Rune
            if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 2 && !ship2ShieldBool)
            {
                tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleShield(2);
                }
            }
            
            //HPup Rune
            else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 3)
            {
                tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleHPUp(2);
                }
            }
            
            //Double Damage Rune
            else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 5)
            {
                tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleDoubleDamage(2);
                }
            }
            
            //Invert Direction Rune
            else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 4)
            {
                tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
                
                if (NetworkEngine::getInstance()->playerNr == 1)
                {
                    toggleInvertDirection(2);
                }
            }
        }
    }
    
    //check if game has started
    if (!loadingLayer->isVisible())
    {
        int positionIterations = 10;
        int velocityIterations = 10;
        
        deltaTime = delta;
        world->Step(delta, velocityIterations, positionIterations);
        
        // Calculate Tangent Vector
        b2Vec2 radius = planetBody->GetPosition() - wormHoleBody->GetPosition();
        b2Vec2 tangent = radius.Skew();
        tangent.Normalize();
        
        // Apply some force along tangent
        wormHoleBody->SetLinearVelocity(10 * tangent);
        
        for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
        {
            b2Vec2 center = planetBody->GetPosition();
            
            b2Vec2 position = body->GetPosition();
            
            // get the distance between the two objects
            b2Vec2 distance = center - position;
            
            // genius equation handle with care
            float force = pow((distance.Length()/3 - 12), 2);
            distance.Normalize();
            
            b2Vec2 F = force * distance;
            // apply a force on the body in the direction of the "Planet"
            body->ApplyForceToCenter(F);
            
            if (body->GetUserData())
            {
                CCSprite *sprite = (CCSprite *) body->GetUserData();
                sprite->setPosition(CCPoint(body->GetPosition().x * 32,body->GetPosition().y * 32));
                sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
            }
        }
    }
    
    //set camera focus depending on players id
    switch (NetworkEngine::getInstance()->playerNr)
    {
        case 1:
            this->setViewPointCenter(CCPoint(shipBody1->GetPosition().x * 32, shipBody1->GetPosition().y *32));
            break;
            
        case 2:
            this->setViewPointCenter(CCPoint(shipBody1->GetPosition().x * 32, shipBody1->GetPosition().y *32));
            break;
            
        case 3:
            this->setViewPointCenter(CCPoint(shipBody2->GetPosition().x * 32, shipBody2->GetPosition().y *32));
            break;
            
        case 4:
            this->setViewPointCenter(CCPoint(shipBody2->GetPosition().x * 32, shipBody2->GetPosition().y *32));
            break;
            
        default:
            break;
    }
    
    //things to be destroyded for memory handling
    std::vector<b2Body *>toDestroy;
    std::vector<MyContact>::iterator pos;
    for(pos = _contactListener->_contacts.begin(); pos != _contactListener->_contacts.end(); ++pos)
    {
        MyContact contact = *pos;
        
        b2Body *bodyA = contact.fixtureA->GetBody();
        b2Body *bodyB = contact.fixtureB->GetBody();
        if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL)
        {
            CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
            CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
            
            // Sprite A = Ship2, Sprite B = Bullet1 or Bullet2
            if ((spriteA->getTag() == 2 && spriteB->getTag() == 3) || ((spriteA->getTag() == 2 && spriteB->getTag() == 4)))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                    CCLOG("Ship 2 has been hit");
                    
                    if (NetworkEngine::getInstance()->playerNr == 1)
                    {
                        if (!ship2ShieldBool)
                        {
                            if (ship2DoubleDamageBool)
                            {
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 2);
                                
                                NetworkEngine::getInstance()->sendEvent(3, eventContent);
                                
                                someOneGotHit(2);
                            }
                            
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 2);
                            
                            NetworkEngine::getInstance()->sendEvent(3, eventContent);
                            
                            someOneGotHit(2);
                        }
                    }
                }
            }
            // Sprite A = Bullet1 or Bullet2, Sprite B = Ship2
            else if ((spriteA->getTag() == 3 && spriteB->getTag() == 2) || (spriteA->getTag() == 4 && spriteB->getTag() == 2))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                    CCLOG("Ship 2 has been hit");
                    
                    if (NetworkEngine::getInstance()->playerNr == 1)
                    {
                        if (!ship2ShieldBool)
                        {
                            if (ship2DoubleDamageBool)
                            {
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 2);
                                
                                NetworkEngine::getInstance()->sendEvent(3, eventContent);
                                
                                someOneGotHit(2);
                            }
                            
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 2);
                            
                            NetworkEngine::getInstance()->sendEvent(3, eventContent);
                            
                            someOneGotHit(2);
                        }
                    }
                }
            }
            
            // Sprite A = Ship1, Sprite B = Bullet2 or Bullet1
            if ((spriteA->getTag() == 1 && spriteB->getTag() == 4) || (spriteA->getTag() == 1 && spriteB->getTag() == 3))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                    CCLOG("Ship 1 has been hit");
                    
                    if (NetworkEngine::getInstance()->playerNr == 1)
                    {
                        if (!ship1ShieldBool)
                        {
                            if (ship1DoubleDamageBool)
                            {
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 1);
                                
                                NetworkEngine::getInstance()->sendEvent(3, eventContent);
                                
                                someOneGotHit(1);
                            }
                            
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 1);
                            
                            NetworkEngine::getInstance()->sendEvent(3, eventContent);
                            
                            someOneGotHit(1);
                        }
                    }
                }
            }
            // Sprite A = Bullet2 or Bullet1, Sprite B = Ship1
            else if ((spriteA->getTag() == 4 && spriteB->getTag() == 1) || (spriteA->getTag() == 3 && spriteB->getTag() == 1))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                    CCLOG("Ship 1 has been hit");
                    
                    if (NetworkEngine::getInstance()->playerNr == 1)
                    {
                        if (!ship1ShieldBool)
                        {
                            if (ship1DoubleDamageBool)
                            {
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 1);
                                
                                NetworkEngine::getInstance()->sendEvent(3, eventContent);
                                
                                someOneGotHit(1);
                            }
                            
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 1);
                            
                            NetworkEngine::getInstance()->sendEvent(3, eventContent);
                            
                            someOneGotHit(1);
                        }
                    }
                }
            }
            
            // Sprite A = Worm Hole, Sprite B = Ship1 or Sprite A = Ship1, Sprite B = WormHole
            else if ((spriteA->getTag() == 5 && spriteB->getTag() == 1)||(spriteA->getTag() == 1 && spriteB->getTag() == 5))
            {
                b2Vec2 warppedPosition = b2Vec2(shipBody1->GetPosition().x * -1, shipBody1->GetPosition().y * -1);
                shipBody1->SetTransform(warppedPosition, shipBody1->GetAngle());
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_jump.mp3");
                
                CCScaleTo* smaller = CCScaleTo::create(0.01, 0.1);
                CCScaleTo* bigger = CCScaleTo::create(0.15, 1);
                CCSequence* warpSeq = CCSequence::create(smaller, bigger, NULL);
                ship1->runAction(warpSeq);
            }
            
            // Sprite A = Worm Hole, Sprite B = Ship2 or Sprite A = Ship2, Sprite B = WormHole
            else if ((spriteA->getTag() == 5 && spriteB->getTag() == 2)||(spriteA->getTag() == 2 && spriteB->getTag() == 5))
            {
                b2Vec2 warppedPosition = b2Vec2(shipBody2->GetPosition().x * -1, shipBody2->GetPosition().y * -1);
                shipBody2->SetTransform(warppedPosition, shipBody2->GetAngle());
                
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_jump.mp3");
                
                CCScaleTo* smaller = CCScaleTo::create(0.01, 0.1);
                CCScaleTo* bigger = CCScaleTo::create(0.15, 1);
                CCSequence* warpSeq = CCSequence::create(smaller, bigger, NULL);
                ship2->runAction(warpSeq);
            }
            
            // Sprite A = Bullet1 or Bullet2, Sprite B = Planet
            else if ((spriteA->getTag() == 3 && spriteB->getTag() == 6) || (spriteA->getTag() == 4 && spriteB->getTag() == 6))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                }
            }
            // Sprite A = Planet, Sprite B = Bullet1 or Bullet2
            else if ((spriteA->getTag() == 6 && spriteB->getTag() == 3) || (spriteA->getTag() == 6 && spriteB->getTag() == 4))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                }
            }
            
            // Sprite A = Bullet1 or Bullet2, Sprite B = WormHole
            else if ((spriteA->getTag() == 3 && spriteB->getTag() == 5) || (spriteA->getTag() == 4 && spriteB->getTag() == 5))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                }
            }
            // Sprite A = WormHole, Sprite B = Bullet1 or Bullet2
            else if ((spriteA->getTag() == 5 && spriteB->getTag() == 3) || (spriteA->getTag() == 5 && spriteB->getTag() == 4))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                }
            }
            
            if (thisGameMode == 2)
            {
                // Sprite A = Ship1 or Ship2, Sprite B = Ship2 or Ship1
                if ((spriteA->getTag() == 1 && spriteB->getTag() == 2) || (spriteA->getTag() == 2 && spriteB->getTag() == 1))
                {
                    if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                    {
                        CCLOG("Ship 1 has been hit");
                        
                        if (NetworkEngine::getInstance()->playerNr == 1)
                        {
                            if (!ship1ShieldBool)
                            {
                                if (ship1DoubleDamageBool)
                                {
                                    ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                    eventContent->put<int, float>(1, 1);
                                    
                                    NetworkEngine::getInstance()->sendEvent(3, eventContent);
                                    
                                    someOneGotHit(1);
                                }
                                
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 1);
                                
                                NetworkEngine::getInstance()->sendEvent(3, eventContent);
                                
                                someOneGotHit(1);
                            }
                            
                        }
                    }
                }
                
            }
        }
    }
    
    //destroy things that are in the list
    std::vector<b2Body *>::iterator pos2;
    for(pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2)
    {
        b2Body *body = *pos2;
        if (body->GetUserData() != NULL)
        {
            CCSprite *sprite = (CCSprite *) body->GetUserData();
            worldLayer->removeChild(sprite, true);
        }
        body->SetActive(false);
        world->DestroyBody(body);
    }
    
    //check if game over
    if ((score1 == 0 || score2 == 0) && !isGameOver)
    {
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            isGameOver = true;
            gameOver();
        }
    }
    
    world->DrawDebugData();
}

//remove waiting room layer and begins game
void GameScene::removeLoading()
{
    loadingLayer->setVisible(false);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_sweep.mp3");
}

//send positions to peers, only to be done by host
void GameScene::sendPositions()
{
    if (NetworkEngine::getInstance()->playerNr)
    {
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, shipBody1->GetPosition().x);
        eventContent->put<int, float>(2, shipBody1->GetPosition().y);
        eventContent->put<int, float>(3, shipBody1->GetAngle());
        eventContent->put<int, float>(4, shipBody2->GetPosition().x);
        eventContent->put<int, float>(5, shipBody2->GetPosition().y);
        eventContent->put<int, float>(6, shipBody2->GetAngle());
        eventContent->put<int, float>(7, wormHoleBody->GetPosition().x);
        eventContent->put<int, float>(8, wormHoleBody->GetPosition().y);

        NetworkEngine::getInstance()->sendEvent(1, eventContent);
    }
}

//game over callback
void GameScene::gameOver()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
    if (NetworkEngine::getInstance()->playerNr == 1)
    {
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, score1);
        eventContent->put<int, float>(2, score2);
        
        NetworkEngine::getInstance()->sendEvent(6, eventContent);
    }
    
    CCLOG("GAME OVER, Score1 %d, Score2 %d", score1, score2);
    
    if (score2 == 0)
    {
        CCTransitionFade* pScene = CCTransitionFade::create(0.5,GameOver::scene(1), ccBLACK);
        CCDirector::sharedDirector()->replaceScene(pScene);
    }
    else if (score1 == 0)
    {
        if (thisGameMode == 2)
        {
            CCTransitionFade* pScene = CCTransitionFade::create(0.5,GameOver::scene(3), ccBLACK);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
        else if (thisGameMode == 4)
        {
            CCTransitionFade* pScene = CCTransitionFade::create(0.5,GameOver::scene(2), ccBLACK);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
    }
}

//ship navigational controls
void GameScene::turn(int playerN, int direction)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_lose.mp3");
    
    b2Vec2 force1 = b2Vec2((cos(shipBody1->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 10) , (sin(shipBody1->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 10));
    b2Vec2 force2 = b2Vec2((cos(shipBody2->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 10) , (sin(shipBody2->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 10));
    
    if (playerN == 1)
    {
        CCLOG("Player 1 Turning");
        
        shipBody1->SetLinearVelocity(force1);
        
        if (direction == 0)
        {
            if (!ship1InvertDirectionBool)
            {
                shipBody1->SetAngularVelocity(-0.9);
            }
            else
            {
                shipBody1->SetAngularVelocity(0.9);
            }
        }
        else if (direction == 1)
        {
            if (!ship1InvertDirectionBool)
            {
                shipBody1->SetAngularVelocity(0.9);
            }
            else
            {
                shipBody1->SetAngularVelocity(-0.9);
            }
        }
        
        ship1flame->setVisible(true);
        CCDelayTime* delay = CCDelayTime::create(1.5);
        CCCallFunc* hideFlame = CCCallFunc::create(this, callfunc_selector(GameScene::hideShip1Flame));
        CCSequence* seq = CCSequence::create(delay, hideFlame, NULL);
        this->runAction(seq);
    }
    else if (playerN == 2)
    {
        CCLOG("Player 2 Turning");
        
        shipBody1->SetLinearVelocity(force1);
        
        if (direction == 0)
        {
            if (!ship1InvertDirectionBool)
            {
                shipBody1->SetAngularVelocity(-0.9);
            }
            else
            {
                shipBody1->SetAngularVelocity(0.9);
            }
        }
        else if (direction == 1)
        {
            if (!ship1InvertDirectionBool)
            {
                shipBody1->SetAngularVelocity(0.9);
            }
            else
            {
                shipBody1->SetAngularVelocity(-0.9);
            }
        }
        
        ship1flame->setVisible(true);
        CCDelayTime* delay = CCDelayTime::create(1.5);
        CCCallFunc* hideFlame = CCCallFunc::create(this, callfunc_selector(GameScene::hideShip1Flame));
        CCSequence* seq = CCSequence::create(delay, hideFlame, NULL);
        this->runAction(seq);
    }
    else if (playerN == 3)
    {
        CCLOG("Player 3 Turning");
        
        shipBody2->SetLinearVelocity(force2);
        
        if (direction == 0)
        {
            if (!ship2InvertDirectionBool)
            {
                shipBody2->SetAngularVelocity(-0.9);
            }
            else
            {
                shipBody2->SetAngularVelocity(0.9);
            }
        }
        else if (direction == 1)
        {
            if (!ship2InvertDirectionBool)
            {
                shipBody2->SetAngularVelocity(0.9);
            }
            else
            {
                shipBody2->SetAngularVelocity(-0.9);
            }
        }
        
        ship2flame->setVisible(true);
        CCDelayTime* delay = CCDelayTime::create(1.5);
        CCCallFunc* hideFlame = CCCallFunc::create(this, callfunc_selector(GameScene::hideShip2Flame));
        CCSequence* seq = CCSequence::create(delay, hideFlame, NULL);
        this->runAction(seq);
    }
    else if (playerN == 4)
    {
        CCLOG("Player 4 Turning");
        
        shipBody2->SetLinearVelocity(force2);
        
        if (direction == 0)
        {
            if (!ship2InvertDirectionBool)
            {
                shipBody2->SetAngularVelocity(-0.9);
            }
            else
            {
                shipBody2->SetAngularVelocity(0.9);
            }
        }
        else if (direction == 1)
        {
            if (!ship2InvertDirectionBool)
            {
                shipBody2->SetAngularVelocity(0.9);
            }
            else
            {
                shipBody2->SetAngularVelocity(-0.9);
            }
        }
        
        ship2flame->setVisible(true);
        CCDelayTime* delay = CCDelayTime::create(1.5);
        CCCallFunc* hideFlame = CCCallFunc::create(this, callfunc_selector(GameScene::hideShip2Flame));
        CCSequence* seq = CCSequence::create(delay, hideFlame, NULL);
        this->runAction(seq);
    }
}

//ship visual effect handler
void GameScene::hideShip1Flame()
{
    ship1flame->setVisible(false);
}

void GameScene::hideShip2Flame()
{
    ship2flame->setVisible(false);
}

//fire projectile
void GameScene::shoot(int playerN)
{
    switch (std::rand()%2)
    {
        case 0:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser1.mp3");
            break;
            
        case 1:
            CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser2.mp3");
            break;
            
        default:
            break;
    }
    
    //bullet shape definition
    b2CircleShape bulletShape;
    bulletShape.m_p.Set(0, 0);
    bulletShape.m_radius = 25/32;
    
    //bullet fixture definition
    b2FixtureDef bulletFixture;
    bulletFixture.density=0.05;
    bulletFixture.isSensor=false;
    bulletFixture.shape=&bulletShape;
    
    if (playerN == 1 || playerN == 2)
    {
        //create bullet
        CCSprite* bullet = CCSprite::create("bullet_blue.png");
        bullet->setPosition(CCPoint((shipBody1->GetPosition().x + cos(shipBody1->GetAngle()-CC_DEGREES_TO_RADIANS(270))*5) *32,
                                    (shipBody1->GetPosition().y + sin(shipBody1->GetAngle()-CC_DEGREES_TO_RADIANS(270))*5) *32));
        bullet->setTag(3);
        worldLayer->addChild(bullet);
        
        //body definition for bullet
        b2BodyDef bulletBodyDef;
        bulletBodyDef.type= b2_dynamicBody;
        bulletBodyDef.userData=bullet;
        bulletBodyDef.position.Set(bullet->getPosition().x/32,bullet->getPosition().y/32);
        
        bulletBody = world->CreateBody(&bulletBodyDef);
        bulletBody->CreateFixture(&bulletFixture);
        bulletBody->SetLinearDamping(0.05);
        bulletBody->IsBullet();
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 30) , (sin(shipBody1->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 30));
        bulletBody->SetLinearVelocity(force);
        
    }
    else if (playerN == 3 || playerN ==4)
    {
        //create bullet
        CCSprite* bullet = CCSprite::create("bullet_green.png");
        bullet->setPosition(CCPoint((shipBody2->GetPosition().x + cos(shipBody2->GetAngle()-CC_DEGREES_TO_RADIANS(270))*5) *32,
                                    (shipBody2->GetPosition().y + sin(shipBody2->GetAngle()-CC_DEGREES_TO_RADIANS(270))*5) *32));
        bullet->setTag(4);
        worldLayer->addChild(bullet);
        
        //body definition for bullet
        b2BodyDef bulletBodyDef;
        bulletBodyDef.type= b2_dynamicBody;
        bulletBodyDef.userData=bullet;
        bulletBodyDef.position.Set(bullet->getPosition().x/32,bullet->getPosition().y/32);
        
        bulletBody = world->CreateBody(&bulletBodyDef);
        bulletBody->CreateFixture(&bulletFixture);
        bulletBody->SetLinearDamping(0.1);
        bulletBody->IsBullet();
        
        b2Vec2 force = b2Vec2((cos(shipBody2->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 30) , (sin(shipBody2->GetAngle()-CC_DEGREES_TO_RADIANS(270)) * 30));
        bulletBody->SetLinearVelocity(force);
    }
    
}

//switch roles
void GameScene::toggleInvertRole()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_powerup.mp3");

    switchRoleLabel->setVisible(true);
    CCActionInterval* actionFadeOut = CCFadeOut::create(2);
    switchRoleLabel->runAction(actionFadeOut);
    
    if (NetworkEngine::getInstance()->playerNr == 1)
    {
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        NetworkEngine::getInstance()->sendEvent(13, eventContent);
    }
    
    if (!isRoleFlipped)
    {
        if (NetworkEngine::getInstance()->playerNr == 1 || NetworkEngine::getInstance()->playerNr == 3)
        {
            fireButton->setEnabled(false);
            turnLeftButton->setEnabled(true);
            turnRightButton->setEnabled(true);
        }
        else if (NetworkEngine::getInstance()->playerNr == 2 || NetworkEngine::getInstance()->playerNr == 4)
        {
            fireButton->setEnabled(true);
            turnLeftButton->setEnabled(false);
            turnRightButton->setEnabled(false);
        }
        
        isRoleFlipped = true;
    }
    else
    {
        if (NetworkEngine::getInstance()->playerNr == 1 || NetworkEngine::getInstance()->playerNr == 3)
        {
            fireButton->setEnabled(true);
            turnLeftButton->setEnabled(false);
            turnRightButton->setEnabled(false);
        }
        else if (NetworkEngine::getInstance()->playerNr == 2 || NetworkEngine::getInstance()->playerNr == 4)
        {
            fireButton->setEnabled(false);
            turnLeftButton->setEnabled(true);
            turnRightButton->setEnabled(true);
        }
        
        isRoleFlipped = false;
    }
    
}

//a ship is damaged
void GameScene::someOneGotHit(int victim)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_twoTone.mp3");
    
    if (victim == 2)
    {
        if (score2 != 0)
        {
            score2--;
        }
        
        CCLOG("Score2 %d",score2);
        updateHpBar();
    }
    else if (victim == 1)
    {
        if (score1 != 0)
        {
            score1--;
        }
        
        CCLOG("Score1 %d",score1);
        updateHpBar();
    }
    
    CCLOG("Someone got hit, Score1 %d, Score2 %d", score1, score2);
    
}

//shield effect
void GameScene::toggleShield(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");
    
    if (ship == 1)
    {
        ship1ShieldBool = true;
        ship1shield->setVisible(true);
        tileMapStatusBlueLayer->setTileGID(4, CCPoint(0, 0));
        
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            NetworkEngine::getInstance()->sendEvent(5, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offShield = CCCallFunc::create(this, callfunc_selector(GameScene::disableShip1Shield));
        CCSequence* seq = CCSequence::create(delay, offShield, NULL);
        this->runAction(seq);
    }
    else if (ship == 2)
    {
        ship2ShieldBool = true;
        ship2shield->setVisible(true);
        tileMapStatusGreenLayer->setTileGID(4, CCPoint(0, 0));
        
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            NetworkEngine::getInstance()->sendEvent(5, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offShield = CCCallFunc::create(this, callfunc_selector(GameScene::disableShip2Shield));
        CCSequence* seq = CCSequence::create(delay, offShield, NULL);
        this->runAction(seq);
    }
    
}

void GameScene::disableShip1Shield()
{
    if (ship1ShieldBool)
    {
        ship1ShieldBool = false;
        ship1shield->setVisible(false);
        tileMapStatusBlueLayer->setTileGID(1, CCPoint(0, 0));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

void GameScene::disableShip2Shield()
{
    if (ship2ShieldBool)
    {
        ship2ShieldBool = false;
        ship2shield->setVisible(false);
        tileMapStatusGreenLayer->setTileGID(1, CCPoint(0, 0));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

//health kit
void GameScene::toggleHPUp(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");
    
    if (ship == 1)
    {
        if (score1 != 0 && score1 != 5)
        {
            score1++;
        }
        
        updateHpBar();
    }
    else if (ship == 2)
    {
        if (score2 != 0 && score2 != 5)
        {
            score2++;
        }
        
        updateHpBar();
    }
    
    CCLOG("HP UP, Score1 %d, Score2 %d", score1, score2);
    
    if (NetworkEngine::getInstance()->playerNr == 1)
    {
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, ship);
        
        NetworkEngine::getInstance()->sendEvent(9, eventContent);
    }
    
}

//double damage
void GameScene::toggleDoubleDamage(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");
    
    if (ship == 1)
    {
        ship1DoubleDamageBool = true;
        ship1->setOpacity(128);
        tileMapStatusBlueLayer->setTileGID(2, CCPoint(0, 0));
        
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            NetworkEngine::getInstance()->sendEvent(10, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offDoubleD = CCCallFunc::create(this, callfunc_selector(GameScene::disableShip1DoubleDamage));
        CCSequence* seq = CCSequence::create(delay, offDoubleD, NULL);
        this->runAction(seq);
    }
    else if (ship == 2)
    {
        ship2DoubleDamageBool = true;
        ship2->setOpacity(128);
        tileMapStatusGreenLayer->setTileGID(2, CCPoint(0, 0));
        
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            NetworkEngine::getInstance()->sendEvent(10, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offDoubleD = CCCallFunc::create(this, callfunc_selector(GameScene::disableShip2DoubleDamage));
        CCSequence* seq = CCSequence::create(delay, offDoubleD, NULL);
        this->runAction(seq);
    }
    
}

void GameScene::disableShip1DoubleDamage()
{
    if (ship1DoubleDamageBool)
    {
        ship1DoubleDamageBool = false;
        ship1->setOpacity(255);
        tileMapStatusBlueLayer->setTileGID(1, CCPoint(0, 0));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

void GameScene::disableShip2DoubleDamage()
{
    if (ship2DoubleDamageBool)
    {
        ship2DoubleDamageBool = false;
        ship2->setOpacity(255);
        tileMapStatusGreenLayer->setTileGID(1, CCPoint(0, 0));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

//sabotage
void GameScene::toggleInvertDirection(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");
    
    if (ship == 1)
    {
        ship1InvertDirectionBool = true;
        tileMapStatusBlueLayer->setTileGID(3, CCPoint(0, 0));
        
        CCTintTo* tint = CCTintTo::create(0.7, 255, 252, 194);
        ship1->runAction(tint);
        
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            NetworkEngine::getInstance()->sendEvent(11, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offInvertR = CCCallFunc::create(this, callfunc_selector(GameScene::disableShip1InvertDirection));
        CCSequence* seq = CCSequence::create(delay, offInvertR, NULL);
        this->runAction(seq);
    }
    else if (ship == 2)
    {
        ship2InvertDirectionBool = true;
        tileMapStatusGreenLayer->setTileGID(3, CCPoint(0, 0));
        
        CCTintTo* tint = CCTintTo::create(0.7, 255, 252, 194);
        ship2->runAction(tint);
        
        if (NetworkEngine::getInstance()->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            NetworkEngine::getInstance()->sendEvent(11, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offInvertR = CCCallFunc::create(this, callfunc_selector(GameScene::disableShip2InvertDirection));
        CCSequence* seq = CCSequence::create(delay, offInvertR, NULL);
        this->runAction(seq);
    }
    
}

void GameScene::disableShip1InvertDirection()
{
    if (ship1InvertDirectionBool)
    {
        ship1InvertDirectionBool = false;
        tileMapStatusBlueLayer->setTileGID(1, CCPoint(0, 0));
        
        CCTintTo* tintback = CCTintTo::create(0.7, 255, 255, 255);
        ship1->runAction(tintback);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

void GameScene::disableShip2InvertDirection()
{
    if (ship1InvertDirectionBool)
    {
        ship2InvertDirectionBool = false;
        tileMapStatusGreenLayer->setTileGID(1, CCPoint(0, 0));
        
        CCTintTo* tintback = CCTintTo::create(0.7, 255, 255, 255);
        ship2->runAction(tintback);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

//spawn runes
void GameScene::spawnRunes()
{
    CCLOG("spawning runes");
    if (NetworkEngine::getInstance()->playerNr == 1)
    {
        int random1 = rand() % 4 + 2;
        // random in the range 2 to 5
        
        tileMapSpawn1Layer->setTileGID(random1, CCPoint(0, 0));
        
        int random2 = rand() % 4 + 2;
        // random in the range 2 to 5
        
        tileMapSpawn2Layer->setTileGID(random2, CCPoint(0, 0));
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, random1);
        eventContent->put<int, float>(2, random2);
        
        NetworkEngine::getInstance()->sendEvent(12, eventContent);
    }
}

//status bar update
void GameScene::updateHpBar()
{
    switch (score1)
    {
        case 5:
            squareBlue1->setVisible(true);
            squareBlue2->setVisible(true);
            squareBlue3->setVisible(true);
            squareBlue4->setVisible(true);
            squareBlue5->setVisible(true);
            break;
            
        case 4:
            squareBlue1->setVisible(true);
            squareBlue2->setVisible(true);
            squareBlue3->setVisible(true);
            squareBlue4->setVisible(true);
            squareBlue5->setVisible(false);
            break;
            
        case 3:
            squareBlue1->setVisible(true);
            squareBlue2->setVisible(true);
            squareBlue3->setVisible(true);
            squareBlue4->setVisible(false);
            squareBlue5->setVisible(false);
            break;
            
        case 2:
            squareBlue1->setVisible(true);
            squareBlue2->setVisible(true);
            squareBlue3->setVisible(false);
            squareBlue4->setVisible(false);
            squareBlue5->setVisible(false);
            break;
            
        case 1:
            squareBlue1->setVisible(true);
            squareBlue2->setVisible(false);
            squareBlue3->setVisible(false);
            squareBlue4->setVisible(false);
            squareBlue5->setVisible(false);
            break;
            
        case 0:
            squareBlue1->setVisible(false);
            squareBlue2->setVisible(false);
            squareBlue3->setVisible(false);
            squareBlue4->setVisible(false);
            squareBlue5->setVisible(false);
            break;
            
        default:
            break;
    }
    
    switch (score2)
    {
        case 5:
            squareGreen1->setVisible(true);
            squareGreen2->setVisible(true);
            squareGreen3->setVisible(true);
            squareGreen4->setVisible(true);
            squareGreen5->setVisible(true);
            break;
            
        case 4:
            squareGreen1->setVisible(true);
            squareGreen2->setVisible(true);
            squareGreen3->setVisible(true);
            squareGreen4->setVisible(true);
            squareGreen5->setVisible(false);
            break;
            
        case 3:
            squareGreen1->setVisible(true);
            squareGreen2->setVisible(true);
            squareGreen3->setVisible(true);
            squareGreen4->setVisible(false);
            squareGreen5->setVisible(false);
            break;
            
        case 2:
            squareGreen1->setVisible(true);
            squareGreen2->setVisible(true);
            squareGreen3->setVisible(false);
            squareGreen4->setVisible(false);
            squareGreen5->setVisible(false);
            break;
            
        case 1:
            squareGreen1->setVisible(true);
            squareGreen2->setVisible(false);
            squareGreen3->setVisible(false);
            squareGreen4->setVisible(false);
            squareGreen5->setVisible(false);
            break;
            
        case 0:
            squareGreen1->setVisible(false);
            squareGreen2->setVisible(false);
            squareGreen3->setVisible(false);
            squareGreen4->setVisible(false);
            squareGreen5->setVisible(false);
            break;
            
        default:
            break;
    }
}

//method to help camera focus
void GameScene::setViewPointCenter(CCPoint position)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint centerOfView = CCPoint(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, position);
    
    worldLayer->setPosition(viewPoint);
}

//firebutton callback
void GameScene::fireButtonCall(CCObject *sender)
{
    CCLOG("Fire Button");
    
    if (heatAmount < 10)
    {
        if (NetworkEngine::getInstance()->playerNr)
        {
            this->shoot(NetworkEngine::getInstance()->playerNr);
            heatAmount = heatAmount + 7;
            CCLOG("heatAmount: %f", heatAmount);
            
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, bulletBody->GetPosition().x);
            eventContent->put<int, float>(2, bulletBody->GetPosition().y);
            eventContent->put<int, float>(3, bulletBody->GetAngle());
            
            NetworkEngine::getInstance()->sendEvent(2, eventContent);
        }
    }
    else
    {
        overHeatLabel->setVisible(true);
        CCActionInterval* actionFadeOut = CCFadeOut::create(0.5);
        overHeatLabel->runAction(actionFadeOut);
    }
}

//AI bot logics
void GameScene::moveSeekingAI()
{
    b2Vec2 target = shipBody1->GetPosition();
    
    b2Vec2 position = shipBody2->GetPosition();
    
    // Get the distance between the two objects.
    b2Vec2 distance = target - position;
    
    distance.Normalize();
    
    b2Vec2 force = 7 * distance;
    
    shipBody2->SetLinearVelocity(force);
}

//turn right button call back
void GameScene::turnRightButtonCall(CCObject *sender)
{
    CCLOG("Turn Button");
    
    if (NetworkEngine::getInstance()->playerNr)
    {
        this->turn(NetworkEngine::getInstance()->playerNr, 0);
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, 0);
        NetworkEngine::getInstance()->sendEvent(4, eventContent);
    }
}

//turn left button call back
void GameScene::turnLeftButtonCall(CCObject *sender)
{
    CCLOG("Turn Button");
    
    if (NetworkEngine::getInstance()->playerNr)
    {
        this->turn(NetworkEngine::getInstance()->playerNr, 1);
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, 1);
        NetworkEngine::getInstance()->sendEvent(4, eventContent);
    }
}

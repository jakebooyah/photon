//
//  TwoPlayerScene.cpp
//  photon
//
//  Created by Jake on 3/28/15.
//
//

#include "TwoPlayerGameScene.h"
#include "CCScale9Sprite.h"
#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include <cstdlib>

USING_NS_CC;

CCScene* TwoPlayerGameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TwoPlayerGameScene *layer = TwoPlayerGameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TwoPlayerGameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 05 The first unfinished song for the Minecraft documentary.mp3", true);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    networkLogic = new NetworkLogic();
    
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    
    world = new b2World(gravity);
    
    worldLayer = CCLayer::create();
    
    CCSprite *background = CCSprite::create("background.png");
    background->setAnchorPoint(CCPoint(0.5, 0.5));
    background->setPosition(CCPoint(1536, 1536));
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
    planet->setPosition(CCPoint(1536, 1536));
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
    
    
    //PowerUp background
    powerUpBg1 = CCSprite::create("powerUp_background.png");
    powerUpBg1->setAnchorPoint(CCPoint(0.5, 0.5));
    powerUpBg1->setPosition(CCPoint(512, 2584));
    worldLayer->addChild(powerUpBg1);
    
    powerUpBg2 = CCSprite::create("powerUp_background.png");
    powerUpBg2->setAnchorPoint(CCPoint(0.5, 0.5));
    powerUpBg2->setPosition(CCPoint(2584, 512));
    worldLayer->addChild(powerUpBg2);
    
    //PowerUP tilemap
    tilemapSpawn1 = CCTMXTiledMap::create("powerUp_tilemap.tmx");
    tileMapSpawn1Layer = tilemapSpawn1->layerNamed("baseLayer");
    tilemapSpawn1->setAnchorPoint(CCPoint(0.5, 0.5));
    tilemapSpawn1->setPosition(CCPoint(512, 2584));
    worldLayer->addChild(tilemapSpawn1);
    
    tilemapSpawn2 = CCTMXTiledMap::create("powerUp_tilemap.tmx");
    tileMapSpawn2Layer = tilemapSpawn2->layerNamed("baseLayer");
    tilemapSpawn2->setAnchorPoint(CCPoint(0.5, 0.5));
    tilemapSpawn2->setPosition(CCPoint(2584, 512));
    worldLayer->addChild(tilemapSpawn2);
    
    
    //ship shape definition
    b2CircleShape shipShape;
    shipShape.m_p.Set(0, 0);
    shipShape.m_radius = 75/32;
    
    //ship fixture definition
    b2FixtureDef shipFixture;
    shipFixture.density=2;
    shipFixture.friction=0;
    shipFixture.restitution=0.5;
    shipFixture.shape=&shipShape;
    
    //create ship 1
    ship1 = CCSprite::create("ship_blue.png");
    ship1->setPosition(CCPoint(512,512));
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
    ship2 = CCSprite::create("ship_ai.png");
    ship2->setPosition(CCPoint(2584, 2584));
    ship2->setTag(2);
    worldLayer->addChild(ship2);
    
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
    
    CCSprite* fireButtonS = CCSprite::create("fireButton.png");
    CCSprite* fireButtonPressedS = CCSprite::create("fireButtonPressed.png");
    CCSprite* fireButtonDisS = CCSprite::create("disabledButton.png");
    
    fireButton = CCMenuItemSprite::create(fireButtonS, fireButtonPressedS, fireButtonDisS, this, menu_selector(TwoPlayerGameScene::fireButtonCall));
    fireButton->setScale(3);
    CCMenu* menuFire = CCMenu::create(fireButton, NULL);
    menuFire->setPosition(CCPoint(visibleSize.width - 250, 250));
    hudLayer->addChild(menuFire);
    
    CCSprite* turnButtonS = CCSprite::create("turnRight.png");
    CCSprite* turnButtonPressedS = CCSprite::create("turnRightPressed.png");
    
    CCMenuItemSprite* turnButton = CCMenuItemSprite::create(turnButtonS, turnButtonPressedS, this, menu_selector(TwoPlayerGameScene::turnButtonCall));
    turnButton->setScale(3);
    CCMenu* menuTurn = CCMenu::create(turnButton, NULL);
    
    menuTurn->setPosition(CCPoint(250, 250));
    hudLayer->addChild(menuTurn);
    
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
    
    
    loadingLayer = CCLayer::create();
    loadingLayer->setTouchPriority(1);
    
    CCSprite *loadingBackground = CCSprite::create("background.png");
    loadingBackground->setAnchorPoint(CCPoint(0.5, 0.5));
    loadingBackground->setScale(2);
    loadingBackground->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    loadingLayer->addChild(loadingBackground);
    
    CCLabelTTF* loading = CCLabelTTF::create("LOADING", "Kenvector Future.ttf", 80);
    loading->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    loadingLayer->addChild(loading);
    
    this->addChild(loadingLayer);
    
    
    //Contact Listener Init
    _contactListener = new ContactListener();
    world->SetContactListener(_contactListener);
    
    score1 = 5;
    score2 = 5;
    
    Player2Joined = false;
    
    ship1ShieldBool = false;
    
    ship1DoubleDamageBool = false;
    
    ship1InvertRoleBool = false;
    
    scheduleUpdate();
    
    return true;
}

void TwoPlayerGameScene::update(float delta)
{
    if (networkLogic->playerNr == 1)
    {
        sendPositions();
    }
    
    moveSeekingAI();
    
    networkLogic->run();
    
    switch (networkLogic->getState())
    {
        case STATE_ROOMFULL:
        {
            networkLogic->setLastInput(INPUT_EXIT);
            
            CCTransitionFade* pScene = CCTransitionFade::create(0.7,MainMenu::scene(), ccBLACK);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
            break;
        case STATE_CONNECTED:
        case STATE_LEFT:
        {
            if (networkLogic->isRoomExists())
            {
                CCLOG("Join");
                networkLogic->setLastInput(INPUT_2);
            }
            else
            {
                CCLOG("Create");
                networkLogic->setLastInput(INPUT_1);
            }
        }
            break;
        case STATE_DISCONNECTED:
            networkLogic->connect();
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
    if (true/*Player2Joined*/)
    {
        //if from Host
        if (networkLogic->playerNr == 1)
        {
            if (loadingLayer->isVisible())
            {
                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                networkLogic->sendEvent(8, eventContent);
                removeLoading();
                
                spawnRunes();
                
                CCDelayTime* delay = CCDelayTime::create(60);
                CCCallFunc* spawnRunes = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::spawnRunes));
                CCSequence* seq = CCSequence::create(delay, spawnRunes, NULL);
                CCRepeatForever* seqLoop = CCRepeatForever::create(seq);
                this->runAction(seqLoop);
            }
        }
    }
    
    while (!networkLogic->eventQueue.empty())
    {
        std::vector<float> arr = networkLogic->eventQueue.front();
        networkLogic->eventQueue.pop();
        
        int code = arr.back();
        arr.pop_back();
        
        switch (code)
        {
            //Update position
            case 1:
            {
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
                    int delay = networkLogic->getRoundTripTime()/100 / 2;
                    
                    if (delay < 3)
                    {
                        b2Vec2 futurePosition = b2Vec2(x1 + velocity.x * delay, y1 + velocity.y * delay);
                        float futureAngle = angle1 + shipBody1->GetAngularVelocity() * delay;
                        
                        shipBody1->SetTransform(futurePosition, futureAngle);
                    }
                    
                    shipBody2->SetTransform(b2Vec2(x2, y2), angle2);
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
                
                if (playerNr != networkLogic->playerNr)
                {
                    this->shoot(playerNr);
                    
                    if (bulletBody->GetUserData() != NULL)
                    {
                        b2Vec2 velocity = bulletBody->GetLinearVelocity();
                        int delay = networkLogic->getRoundTripTime()/100 / 2;
                        
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
                int playerNr = arr.back();
                arr.pop_back();
                
                this->turn(playerNr);
                
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
                int playerNr = arr.back();
                arr.pop_back();
                
                if (networkLogic->playerNr == 1)
                {
                    if (playerNr == 2)
                    {
                        CCLOG("Player 2 joined");
                        Player2Joined = true;
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
            //Invert Role
            case 11:
            {
                int ship = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                if (playerNr == 1)
                {
                    this->toggleInvertRole(ship);
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
            
            if (networkLogic->playerNr == 1)
            {
                toggleShield(1);
                CCLOG("Toggle shield 1");
            }
        }
        
        //HPup Rune
        else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 3)
        {
            tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
            
            if (networkLogic->playerNr == 1)
            {
                toggleHPUp(1);
                CCLOG("Toggle Hp UP 1");
            }
        }
        
        //Double Damage Rune
        else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 5)
        {
            tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
            
            if (networkLogic->playerNr == 1)
            {
                toggleDoubleDamage(1);
                CCLOG("Toggle DD 1");
            }
        }
        
        //Invert Role Rune
        else if (tileMapSpawn1Layer->tileGIDAt(CCPoint(0, 0)) == 4)
        {
            tileMapSpawn1Layer->setTileGID(1, CCPoint(0, 0));
            
            if (networkLogic->playerNr == 1)
            {
                toggleInvertRole(1);
                CCLOG("Toggle invert role 1");
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
            
            if (networkLogic->playerNr == 1)
            {
                toggleShield(1);
            }
        }
        
        //HPup Rune
        else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 3)
        {
            tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
            
            if (networkLogic->playerNr == 1)
            {
                toggleHPUp(1);
            }
        }
        
        //Double Damage Rune
        else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 5)
        {
            tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
            
            if (networkLogic->playerNr == 1)
            {
                toggleDoubleDamage(1);
            }
        }
        
        //Invert Role Rune
        else if (tileMapSpawn2Layer->tileGIDAt(CCPoint(0, 0)) == 4)
        {
            tileMapSpawn2Layer->setTileGID(1, CCPoint(0, 0));
            
            if (networkLogic->playerNr == 1)
            {
                toggleInvertRole(1);
            }
        }
    }
    
    int positionIterations = 10;
    int velocityIterations = 10;
    
    deltaTime = delta;
    world->Step(delta, velocityIterations, positionIterations);
    
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
    {
        b2Vec2 center = planetBody->GetPosition();
        
        b2Vec2 position = body->GetPosition();
        
        // Get the distance between the two objects.
        b2Vec2 distance = center - position;
        
        float force =  pow((distance.Length()/4 - 12), 2); //genius equation handle with care radioactive
        distance.Normalize();
        
        b2Vec2 F = force * distance;
        // Finally apply a force on the body in the direction of the "Planet"
        body->ApplyForceToCenter(F);
        
        if (body->GetUserData())
        {
            CCSprite *sprite = (CCSprite *) body->GetUserData();
            sprite->setPosition(CCPoint(body->GetPosition().x * 32,body->GetPosition().y * 32));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
        }
    }
    
    switch (networkLogic->playerNr)
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
                    
                    if (networkLogic->playerNr == 1)
                    {
                        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                        eventContent->put<int, float>(1, 2);
                        
                        networkLogic->sendEvent(3, eventContent);
                        
                        someOneGotHit(2);
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
                    
                    if (networkLogic->playerNr == 1)
                    {
                        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                        eventContent->put<int, float>(1, 2);
                        
                        networkLogic->sendEvent(3, eventContent);
                        
                        someOneGotHit(2);
                    }
                }
            }
            
            // Sprite A = Ship1, Sprite B = Bullet2 or Bullet1
            if ((spriteA->getTag() == 1 && spriteB->getTag() == 4) ||
                (spriteA->getTag() == 1 && spriteB->getTag() == 3))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                    CCLOG("Ship 1 has been hit");
                    
                    if (networkLogic->playerNr == 1)
                    {
                        if (!ship1ShieldBool)
                        {
                            if (ship1DoubleDamageBool)
                            {
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 1);
                                
                                networkLogic->sendEvent(3, eventContent);
                                
                                someOneGotHit(1);
                            }
                            
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 1);
                            
                            networkLogic->sendEvent(3, eventContent);
                            
                            someOneGotHit(1);
                        }
                    }
                }
            }
            // Sprite A = Bullet2 or Bullet1, Sprite B = Ship1
            else if ((spriteA->getTag() == 4 && spriteB->getTag() == 1) ||
                     (spriteA->getTag() == 3 && spriteB->getTag() == 1))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                    CCLOG("Ship 1 has been hit");
                    
                    if (networkLogic->playerNr == 1)
                    {
                        if (!ship1ShieldBool)
                        {
                            if (ship1DoubleDamageBool)
                            {
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 1);
                                
                                networkLogic->sendEvent(3, eventContent);
                                
                                someOneGotHit(1);
                            }
                            
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 1);
                            
                            networkLogic->sendEvent(3, eventContent);
                            
                            someOneGotHit(1);
                        }
                    }
                }
            }
            
            // Sprite A = Ship1 or Ship2, Sprite B = Ship2 or Ship1
            if ((spriteA->getTag() == 1 && spriteB->getTag() == 2) ||
                (spriteA->getTag() == 2 && spriteB->getTag() == 1))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                {
                    CCLOG("Ship 1 has been hit");
                    
                    if (networkLogic->playerNr == 1)
                    {
                        if (!ship1ShieldBool)
                        {
                            if (ship1DoubleDamageBool)
                            {
                                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                                eventContent->put<int, float>(1, 1);
                                
                                networkLogic->sendEvent(3, eventContent);
                                
                                someOneGotHit(1);
                            }
                            
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 1);
                            
                            networkLogic->sendEvent(3, eventContent);
                            
                            someOneGotHit(1);
                        }
                    }
                }
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
        }
    }
    
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
    
    if (score1 == 0 || score2 == 0)
    {
        if (networkLogic->playerNr == 1)
        {
            gameOver();
        }
    }
    
    world->DrawDebugData();
}

void TwoPlayerGameScene::removeLoading()
{
    loadingLayer->setVisible(false);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser1.mp3");
}

void TwoPlayerGameScene::sendPositions()
{
    if (networkLogic->playerNr)
    {
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, shipBody1->GetPosition().x);
        eventContent->put<int, float>(2, shipBody1->GetPosition().y);
        eventContent->put<int, float>(3, shipBody1->GetAngle());
        eventContent->put<int, float>(4, shipBody2->GetPosition().x);
        eventContent->put<int, float>(5, shipBody2->GetPosition().y);
        eventContent->put<int, float>(6, shipBody2->GetAngle());
        
        networkLogic->sendEvent(1, eventContent);
    }
}

void TwoPlayerGameScene::gameOver()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_lose.mp3");
    
    if (networkLogic->playerNr == 1)
    {
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, score1);
        eventContent->put<int, float>(2, score2);
        
        networkLogic->sendEvent(6, eventContent);
    }
    
    CCLOG("GAME OVER, Score1 %d, Score2 %d", score1, score2);
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.7,GameOver::scene(), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void TwoPlayerGameScene::turn(int playerN)
{
    if (playerN == 1)
    {
        CCLOG("Player 1 Turning");
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 10) , (sin(shipBody1->GetAngle()-4.7) * 10));
        
        shipBody1->SetLinearVelocity(force);
        
        if (!ship1InvertRoleBool)
        {
            shipBody1->SetAngularVelocity(-0.5);
        }
        else
        {
            shipBody1->SetAngularVelocity(0.5);
        }
        
        ship1flame->setVisible(true);
        CCDelayTime* delay = CCDelayTime::create(1);
        CCCallFunc* hideFlame = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::hideShip1Flame));
        CCSequence* seq = CCSequence::create(delay, hideFlame, NULL);
        this->runAction(seq);
        
    }
    else if (playerN == 2)
    {
        CCLOG("Player 2 Turning");
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 10) , (sin(shipBody1->GetAngle()-4.7) * 10));
        
        shipBody1->SetLinearVelocity(force);
        
        if (!ship1InvertRoleBool)
        {
            shipBody1->SetAngularVelocity(0.5);
        }
        else
        {
            shipBody1->SetAngularVelocity(-0.5);
        }
        
        ship1flame->setVisible(true);
        CCDelayTime* delay = CCDelayTime::create(1);
        CCCallFunc* hideFlame = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::hideShip1Flame));
        CCSequence* seq = CCSequence::create(delay, hideFlame, NULL);
        this->runAction(seq);
    }
}

void TwoPlayerGameScene::hideShip1Flame()
{
    ship1flame->setVisible(false);
}

void TwoPlayerGameScene::shoot(int playerN)
{
    //bullet shape definition
    b2CircleShape bulletShape;
    bulletShape.m_p.Set(0, 0);
    bulletShape.m_radius = 25/32;
    
    //bullet fixture definition
    b2FixtureDef bulletFixture;
    bulletFixture.density=0.1;
    bulletFixture.isSensor=false;
    bulletFixture.shape=&bulletShape;
    
    if (playerN == 1 || playerN == 2)
    {
        //create bullet
        CCSprite* bullet = CCSprite::create("bullet_blue.png");
        bullet->setPosition(CCPoint((shipBody1->GetPosition().x + cos(shipBody1->GetAngle()-4.7)*5) *32,
                                    (shipBody1->GetPosition().y + sin(shipBody1->GetAngle()-4.7)*5) *32));
        bullet->setTag(3);
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
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 30) , (sin(shipBody1->GetAngle()-4.7) * 30));
        bulletBody->SetLinearVelocity(force);
        
    }
    
    CCCallFunc* disable = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::disableFireButton));
    CCCallFunc* enable = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::enableFireButton));
    CCDelayTime* delay = CCDelayTime::create(2);
    CCSequence* seq = CCSequence::create(disable, delay, enable, NULL);
    this->runAction(seq);
}

void TwoPlayerGameScene::disableFireButton()
{
    fireButton->setEnabled(false);
}

void TwoPlayerGameScene::enableFireButton()
{
    fireButton->setEnabled(true);
}

void TwoPlayerGameScene::someOneGotHit(int victim)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
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

void TwoPlayerGameScene::toggleShield(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");
    
    if (ship == 1)
    {
        ship1ShieldBool = true;
        ship1shield->setVisible(true);
        tileMapStatusBlueLayer->setTileGID(4, CCPoint(0, 0));
        
        if (networkLogic->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            networkLogic->sendEvent(5, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offShield = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::disableShip1Shield));
        CCSequence* seq = CCSequence::create(delay, offShield, NULL);
        this->runAction(seq);
    }
}

void TwoPlayerGameScene::disableShip1Shield()
{
    if (ship1ShieldBool)
    {
        ship1ShieldBool = false;
        ship1shield->setVisible(false);
        tileMapStatusBlueLayer->setTileGID(1, CCPoint(0, 0));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

void TwoPlayerGameScene::toggleHPUp(int ship)
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
    
    if (networkLogic->playerNr == 1)
    {
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, ship);
        
        networkLogic->sendEvent(9, eventContent);
    }
    
}

void TwoPlayerGameScene::toggleDoubleDamage(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");
    
    if (ship == 1)
    {
        ship1DoubleDamageBool = true;
        ship1->setOpacity(128);
        tileMapStatusBlueLayer->setTileGID(2, CCPoint(0, 0));
        
        if (networkLogic->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            networkLogic->sendEvent(10, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offDoubleD = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::disableShip1DoubleDamage));
        CCSequence* seq = CCSequence::create(delay, offDoubleD, NULL);
        this->runAction(seq);
    }
}

void TwoPlayerGameScene::disableShip1DoubleDamage()
{
    if (ship1DoubleDamageBool)
    {
        ship1DoubleDamageBool = false;
        ship1->setOpacity(255);
        tileMapStatusBlueLayer->setTileGID(1, CCPoint(0, 0));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

void TwoPlayerGameScene::toggleInvertRole(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");
    
    if (ship == 1)
    {
        ship1InvertRoleBool = true;
        tileMapStatusBlueLayer->setTileGID(3, CCPoint(0, 0));
        
        if (networkLogic->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            networkLogic->sendEvent(11, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(13);
        CCCallFunc* offInvertR = CCCallFunc::create(this, callfunc_selector(TwoPlayerGameScene::disableShip1InvertRole));
        CCSequence* seq = CCSequence::create(delay, offInvertR, NULL);
        this->runAction(seq);
    }
}

void TwoPlayerGameScene::disableShip1InvertRole()
{
    if (ship1InvertRoleBool)
    {
        ship1InvertRoleBool = false;
        tileMapStatusBlueLayer->setTileGID(1, CCPoint(0, 0));
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

void TwoPlayerGameScene::spawnRunes()
{
    CCLOG("spawning runes");
    if (networkLogic->playerNr == 1)
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
        
        networkLogic->sendEvent(12, eventContent);
    }
}

void TwoPlayerGameScene::updateHpBar()
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

void TwoPlayerGameScene::moveSeekingAI()
{
    b2Vec2 target = shipBody1->GetPosition();
    
    b2Vec2 position = shipBody2->GetPosition();
    
    // Get the distance between the two objects.
    b2Vec2 distance = target - position;
    
    distance.Normalize();
    
    b2Vec2 force = 7 * distance;
    
    shipBody2->SetLinearVelocity(force);
}

void TwoPlayerGameScene::setViewPointCenter(CCPoint position)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCPoint centerOfView = CCPoint(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, position);
    
    worldLayer->setPosition(viewPoint);
}

void TwoPlayerGameScene::fireButtonCall(CCObject *sender)
{
    CCLOG("Fire Button");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser1.mp3");
    
    if (networkLogic->playerNr)
    {
        this->shoot(networkLogic->playerNr);
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, bulletBody->GetPosition().x);
        eventContent->put<int, float>(2, bulletBody->GetPosition().y);
        eventContent->put<int, float>(3, bulletBody->GetAngle());
        
        networkLogic->sendEvent(2, eventContent);
    }
}

void TwoPlayerGameScene::turnButtonCall(CCObject *sender)
{
    CCLOG("Turn Button");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_twoTone.mp3");
    
    if (networkLogic->playerNr)
    {
        this->turn(networkLogic->playerNr);
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        networkLogic->sendEvent(4, eventContent);
    }
}

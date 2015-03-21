#include "HelloWorldScene.h"
#include "CCScale9Sprite.h"
#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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

    //background init
    CCSprite *background = CCSprite::create("bg.png");
    background->setAnchorPoint(CCPoint(0, 0));
    background->setScale(1.5);
    worldLayer->addChild(background);
    
    
    //create wall
    CCSprite* wallCeiling = CCSprite::create("boundary.png");
    wallCeiling->setPosition(CCPoint(1536, 3072));
    wallCeiling->setScale(1.5);
    wallCeiling->setTag(5);
    worldLayer->addChild(wallCeiling);
    
    CCSprite* wallGround = CCSprite::create("boundary.png");
    wallGround->setPosition(CCPoint(1536, 0));
    wallGround->setScale(1.5);
    wallGround->setTag(5);
    worldLayer->addChild(wallGround);
    
    CCSprite* wallLeft = CCSprite::create("boundaryLR.png");
    wallLeft->setPosition(CCPoint(0, 1536));
    wallLeft->setScale(1.5);
    wallLeft->setTag(5);
    worldLayer->addChild(wallLeft);
    
    CCSprite* wallRight = CCSprite::create("boundaryLR.png");
    wallRight->setPosition(CCPoint(3072, 1536));
    wallRight->setScale(1.5);
    wallRight->setTag(5);
    worldLayer->addChild(wallRight);
    
    
    
    //shape definition
    b2PolygonShape wallUDShape;
    wallUDShape.SetAsBox(3072/32, 48/32);
    
    b2FixtureDef wallUDFixtureDef;
    wallUDFixtureDef.shape = &wallUDShape;
    
    b2PolygonShape wallLRShape;
    wallLRShape.SetAsBox(48/32, 3072/32);
    
    b2FixtureDef wallLRFixtureDef;
    wallLRFixtureDef.shape = &wallLRShape;
    
    
    
    //body definition for wallCeling
    b2BodyDef wallUBodyDef;
    wallUBodyDef.type= b2_staticBody;
    wallUBodyDef.userData=wallCeiling;
    wallUBodyDef.position.Set(wallCeiling->getPosition().x/32,wallCeiling->getPosition().y/32);
    
    b2Body* wallUBody = world->CreateBody(&wallUBodyDef);
    wallUBody->CreateFixture(&wallUDFixtureDef);
    wallUBody->SetGravityScale(10);

    //body definition for wallGround
    b2BodyDef wallDBodyDef;
    wallDBodyDef.type= b2_staticBody;
    wallDBodyDef.userData=wallGround;
    wallDBodyDef.position.Set(wallGround->getPosition().x/32,wallGround->getPosition().y/32);
    
    b2Body* wallDBody = world->CreateBody(&wallDBodyDef);
    wallDBody->CreateFixture(&wallUDFixtureDef);
    wallDBody->SetGravityScale(10);
    
    //body definition for wallLeft
    b2BodyDef wallLBodyDef;
    wallLBodyDef.type= b2_staticBody;
    wallLBodyDef.userData=wallLeft;
    wallLBodyDef.position.Set(wallLeft->getPosition().x/32,wallLeft->getPosition().y/32);
    
    b2Body* wallLBody = world->CreateBody(&wallLBodyDef);
    wallLBody->CreateFixture(&wallLRFixtureDef);
    wallLBody->SetGravityScale(10);
    
    //body definition for wallRight
    b2BodyDef wallRBodyDef;
    wallRBodyDef.type= b2_staticBody;
    wallRBodyDef.userData=wallRight;
    wallRBodyDef.position.Set(wallRight->getPosition().x/32,wallRight->getPosition().y/32);
    
    b2Body* wallRBody = world->CreateBody(&wallRBodyDef);
    wallRBody->CreateFixture(&wallLRFixtureDef);
    wallRBody->SetGravityScale(10);
    
    
    
    //planet shape definition
    b2CircleShape planetShape;
    planetShape.m_p.Set(0, 0);
    planetShape.m_radius = 430/32;
    
    //planet fixture definition
    b2FixtureDef planetFixture;
    planetFixture.density=10;
    planetFixture.friction=0;
    planetFixture.restitution=0;
    planetFixture.shape=&planetShape;
    
    //create planet
    planet = CCSprite::create("planet.png");
    planet->setPosition(CCPoint(1536, 1536));
    planet->setScale(2);
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
    
    
    
    //ship shape definition
    b2CircleShape shipShape;
    shipShape.m_p.Set(0, 0);
    shipShape.m_radius = 100/32;
    
    //ship fixture definition
    b2FixtureDef shipFixture;
    shipFixture.density=1;
    shipFixture.friction=0;
    shipFixture.restitution=0;
    shipFixture.shape=&shipShape;
    
    //create ship 1
    ship1 = CCSprite::create("playerShip2_blue.png");
    ship1->setPosition(CCPoint(512,512));
    ship1->setScale(2);
    ship1->setTag(1);
    worldLayer->addChild(ship1);
    
    ship1damage1 = CCSprite::create("playerShip2_damage1.png");
    ship1damage1->setPosition(CCPoint(56, 38));
    ship1damage1->setVisible(false);
    ship1->addChild(ship1damage1);
    
    ship1damage2 = CCSprite::create("playerShip2_damage2.png");
    ship1damage2->setPosition(CCPoint(56, 38));
    ship1damage2->setVisible(false);
    ship1->addChild(ship1damage2);
    
    ship1damage3 = CCSprite::create("playerShip2_damage3.png");
    ship1damage3->setPosition(CCPoint(56, 38));
    ship1damage3->setVisible(false);
    ship1->addChild(ship1damage3);
    
    ship1shield = CCSprite::create("shield1.png");
    ship1shield->setPosition(CCPoint(56, 38));
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
    ship2 = CCSprite::create("playerShip2_green.png");
    ship2->setPosition(CCPoint(2584, 2584));
    ship2->setScale(2);
    ship2->setTag(2);
    worldLayer->addChild(ship2);
    
    ship2damage1 = CCSprite::create("playerShip2_damage1.png");
    ship2damage1->setPosition(CCPoint(56, 38));
    ship2damage1->setVisible(false);
    ship2->addChild(ship2damage1);
    
    ship2damage2 = CCSprite::create("playerShip2_damage2.png");
    ship2damage2->setPosition(CCPoint(56, 38));
    ship2damage2->setVisible(false);
    ship2->addChild(ship2damage2);
    
    ship2damage3 = CCSprite::create("playerShip2_damage3.png");
    ship2damage3->setPosition(CCPoint(56, 38));
    ship2damage3->setVisible(false);
    ship2->addChild(ship2damage3);
    
    ship2shield = CCSprite::create("shield1.png");
    ship2shield->setPosition(CCPoint(56, 38));
    ship2shield->setVisible(false);
    ship2->addChild(ship2shield);
    
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

    fireButton = CCMenuItemSprite::create(fireButtonS, fireButtonPressedS, fireButtonDisS, this, menu_selector(HelloWorld::fireButtonCall));
    fireButton->setScale(3);
    CCMenu* menuFire = CCMenu::create(fireButton, NULL);
    menuFire->setPosition(CCPoint(visibleSize.width - 250, 250));
    hudLayer->addChild(menuFire);
    
    
    CCSprite* turnButtonS = CCSprite::create("turnRight.png");
    CCSprite* turnButtonPressedS = CCSprite::create("turnRightPressed.png");
    
    CCMenuItemSprite* turnButton = CCMenuItemSprite::create(turnButtonS, turnButtonPressedS, this, menu_selector(HelloWorld::turnButtonCall));
    turnButton->setScale(3);
    CCMenu* menuTurn = CCMenu::create(turnButton, NULL);
    
    menuTurn->setPosition(CCPoint(250, 250));
    hudLayer->addChild(menuTurn);
    
    
    cocos2d::extension::CCScale9Sprite* panel = cocos2d::extension::CCScale9Sprite::create("glassPanel.png");
    panel->setContentSize(CCSize(1000, 150));
    panel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height-150));
    hudLayer->addChild(panel);
    
    CCLabelTTF* statusLabel = CCLabelTTF::create("STATUS", "Kenvector Future.ttf", 50);
    statusLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height-150));
    hudLayer->addChild(statusLabel);
    
    for (int n = 0; n < 5; n++)
    {
        CCSprite* squareShadow = CCSprite::create("square_shadow.png");
        squareShadow->setScale(2);
        squareShadow->setPosition(CCPoint(visibleSize.width/2 - 200 - n*50, visibleSize.height-150));
        hudLayer->addChild(squareShadow);
    }
    
    for (int n = 0; n < 5; n++)
    {
        CCSprite* squareShadow = CCSprite::create("square_shadow.png");
        squareShadow->setScale(2);
        squareShadow->setPosition(CCPoint(visibleSize.width/2 + 200 + n*50, visibleSize.height-150));
        hudLayer->addChild(squareShadow);
    }
    
    squareBlue1 = CCSprite::create("squareBlue.png");
    squareBlue1->setScale(2);
    squareBlue1->setPosition(CCPoint(visibleSize.width/2 - 200 - 0*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue1);
    
    squareBlue2 = CCSprite::create("squareBlue.png");
    squareBlue2->setScale(2);
    squareBlue2->setPosition(CCPoint(visibleSize.width/2 - 200 - 1*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue2);
    
    squareBlue3 = CCSprite::create("squareBlue.png");
    squareBlue3->setScale(2);
    squareBlue3->setPosition(CCPoint(visibleSize.width/2 - 200 - 2*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue3);
    
    squareBlue4 = CCSprite::create("squareBlue.png");
    squareBlue4->setScale(2);
    squareBlue4->setPosition(CCPoint(visibleSize.width/2 - 200 - 3*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue4);
    
    squareBlue5 = CCSprite::create("squareBlue.png");
    squareBlue5->setScale(2);
    squareBlue5->setPosition(CCPoint(visibleSize.width/2 - 200 - 4*50, visibleSize.height-150));
    hudLayer->addChild(squareBlue5);
    
    
    
    squareGreen1 = CCSprite::create("squareGreen.png");
    squareGreen1->setScale(2);
    squareGreen1->setPosition(CCPoint(visibleSize.width/2 + 200 + 0*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen1);
    
    squareGreen2 = CCSprite::create("squareGreen.png");
    squareGreen2->setScale(2);
    squareGreen2->setPosition(CCPoint(visibleSize.width/2 + 200 + 1*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen2);
    
    squareGreen3 = CCSprite::create("squareGreen.png");
    squareGreen3->setScale(2);
    squareGreen3->setPosition(CCPoint(visibleSize.width/2 + 200 + 2*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen3);
    
    squareGreen4 = CCSprite::create("squareGreen.png");
    squareGreen4->setScale(2);
    squareGreen4->setPosition(CCPoint(visibleSize.width/2 + 200 + 3*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen4);
    
    squareGreen5 = CCSprite::create("squareGreen.png");
    squareGreen5->setScale(2);
    squareGreen5->setPosition(CCPoint(visibleSize.width/2 + 200 + 4*50, visibleSize.height-150));
    hudLayer->addChild(squareGreen5);
    
    
    this->addChild(hudLayer);
    
    
    loadingLayer = CCLayer::create();
    loadingLayer->setTouchPriority(1);
    
    CCSprite *loadingBackground = CCSprite::create("bg.png");
    loadingBackground->setAnchorPoint(CCPoint(0, 0));
    loadingBackground->setScale(1.5);
    loadingLayer->addChild(loadingBackground);
    
    CCLabelTTF* loading = CCLabelTTF::create("LOADING", "Kenvector Future.ttf", 80);
    loading->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    loadingLayer->addChild(loading);
    
    
    this->addChild(loadingLayer);
    
    
    //Contact Listener Init
    _contactListener = new ContactListener();
    world->SetContactListener(_contactListener);
    
    
    CCDelayTime* delay = CCDelayTime::create(3);
    CCCallFunc* sendPositions = CCCallFunc::create(this, callfunc_selector(HelloWorld::sendPositions));
    CCSequence* seq = CCSequence::create(delay, sendPositions, NULL);
    CCRepeatForever* seqLoop = CCRepeatForever::create(seq);
    this->runAction(seqLoop);

    
    score1 = 5;
    score2 = 5;
    
    Player2Joined = false;
    Player3Joined = false;
    Player4Joined = false;

    scheduleUpdate();
        
    return true;
}

void HelloWorld::update(float delta)
{
    networkLogic->run();
    
    switch (networkLogic->getState())
    {
        case STATE_ROOMFULL:
            {
                networkLogic->setLastInput(INPUT_EXIT);

                CCTransitionFade* pScene = CCTransitionFade::create(0.7,MainMenu::scene(), ccWHITE);
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
    if (true /*Player2Joined && Player3Joined && Player4Joined*/)
    {
        //if from Host
        if (networkLogic->playerNr == 1)
        {
            if (this->loadingLayer->isVisible())
            {
                ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                networkLogic->sendEvent(8, eventContent);
                removeLoading();
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
                    CCLOG("Round Trip Time = %d", networkLogic->getRoundTripTime());
                    
                    //Correction of Ship 1 with dead reckoning
                    
                    b2Vec2 velocity = shipBody1->GetLinearVelocity();
                    int delay = networkLogic->getRoundTripTime()/100 / 2;
                    
                    b2Vec2 futurePosition = b2Vec2(x1 + velocity.x * delay, y1 + velocity.y * delay);
                    float futureAngle = angle1 + shipBody1->GetAngularVelocity() * delay;
                    
                    shipBody1->SetTransform(futurePosition, futureAngle);
                    
                    //Correction of Ship 2 with dead reckoning
                    
                    velocity = shipBody2->GetLinearVelocity();
                    delay = networkLogic->getRoundTripTime()/100 / 2;
                    
                    futurePosition = b2Vec2(x2 + velocity.x * delay, y2 + velocity.y * delay);
                    futureAngle = angle2 + shipBody2->GetAngularVelocity() * delay;

                    shipBody2->SetTransform(futurePosition, futureAngle);
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
                    
                    if (playerNr == 3)
                    {
                        CCLOG("Player 3 joined");
                        Player3Joined = true;
                    }
                    
                    if (playerNr == 4)
                    {
                        CCLOG("Player 4 joined");
                        Player4Joined = true;
                    }
                }
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
            }
            
            default:
                break;
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
        
        // The further away the objects are, the weaker the gravitational force is
        float force = 10000 / distance.LengthSquared(); // can be changed to adjust the amount of force
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
                        if (!ship2ShieldBool)
                        {
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 2);
                            
                            networkLogic->sendEvent(3, eventContent);
                            
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

                    if (networkLogic->playerNr == 1)
                    {
                        if (!ship2ShieldBool)
                        {
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 2);
                            
                            networkLogic->sendEvent(3, eventContent);
                            
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

                    if (networkLogic->playerNr == 1)
                    {
                        if (!ship1ShieldBool)
                        {
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 1);
                            
                            networkLogic->sendEvent(3, eventContent);
                            
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

                    if (networkLogic->playerNr == 1)
                    {
                        if (!ship1ShieldBool)
                        {
                            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                            eventContent->put<int, float>(1, 1);
                            
                            networkLogic->sendEvent(3, eventContent);
                            
                            someOneGotHit(1);
                        }
                    }
                }
            }
            
            // Sprite A = Bullet1 or Bullet2, Sprite B = Wall
            else if ((spriteA->getTag() == 3 && spriteB->getTag() == 5) || (spriteA->getTag() == 4 && spriteB->getTag() == 5))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                }
            }
            // Sprite A = Wall, Sprite B = Bullet1 or Bullet2
            else if ((spriteA->getTag() == 5 && spriteB->getTag() == 3) || (spriteA->getTag() == 5 && spriteB->getTag() == 4))
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
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
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, score1);
            eventContent->put<int, float>(1, score2);

            networkLogic->sendEvent(6, eventContent);
            
            gameOver();
        }
    }
    
    world->DrawDebugData();
}

void HelloWorld::removeLoading()
{
    loadingLayer->setVisible(false);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser1.mp3");
}

void HelloWorld::sendPositions()
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

void HelloWorld::gameOver()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_lose.mp3");
        
    CCTransitionFade* pScene = CCTransitionFade::create(0.7,GameOver::scene(), ccWHITE);
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void HelloWorld::turn(int playerN)
{
    if (playerN == 1)
    {
        CCLOG("Player 1 Turning Left");
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 5) , (sin(shipBody1->GetAngle()-4.7) * 5));

        shipBody1->SetLinearVelocity(force);
        shipBody1->SetAngularVelocity(0.5);
    }
    else if (playerN == 2)
    {
        CCLOG("Player 2 Turning Right");
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 5) , (sin(shipBody1->GetAngle()-4.7) * 5));

        shipBody1->SetLinearVelocity(force);
        shipBody1->SetAngularVelocity(-0.5);
    }
    else if (playerN == 3)
    {
        CCLOG("Player 3 Turning Right");
        
        b2Vec2 force = b2Vec2((cos(shipBody2->GetAngle()-4.7) * 5) , (sin(shipBody2->GetAngle()-4.7) * 5));
        
        shipBody2->SetLinearVelocity(force);
        shipBody2->SetAngularVelocity(0.5);
    }
    else if (playerN == 4)
    {
        CCLOG("Player 4 Turning Right");
        
        b2Vec2 force = b2Vec2((cos(shipBody2->GetAngle()-4.7) * 5) , (sin(shipBody2->GetAngle()-4.7) * 5));
        
        shipBody2->SetLinearVelocity(force);
        shipBody2->SetAngularVelocity(-0.5);
    }
}

void HelloWorld::shoot(int playerN)
{
    //bullet shape definition
    b2CircleShape bulletShape;
    bulletShape.m_p.Set(0, 0);
    bulletShape.m_radius = 40/32;
    
    //bullet fixture definition
    b2FixtureDef bulletFixture;
    bulletFixture.density=0.1;
    bulletFixture.friction=0.5;
    bulletFixture.restitution=0.5;
    bulletFixture.isSensor=false;
    bulletFixture.shape=&bulletShape;
    
    if (playerN == 1 || playerN == 2)
    {
        //create bullet
        CCSprite* bullet = CCSprite::create("laserBlue08.png");
        bullet->setPosition(CCPoint((shipBody1->GetPosition().x + cos(shipBody1->GetAngle()-4.7)*5) *32,
                                    (shipBody1->GetPosition().y + sin(shipBody1->GetAngle()-4.7)*5) *32));
        bullet->setScale(1);
        bullet->setTag(3);
        worldLayer->addChild(bullet);
        
        //body definition for bullet
        b2BodyDef bulletBodyDef;
        bulletBodyDef.type= b2_dynamicBody;
        bulletBodyDef.userData=bullet;
        bulletBodyDef.position.Set(bullet->getPosition().x/32,bullet->getPosition().y/32);
        
        bulletBody = world->CreateBody(&bulletBodyDef);
        bulletBody->CreateFixture(&bulletFixture);
        bulletBody->SetGravityScale(1);
        bulletBody->IsBullet();
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 30) , (sin(shipBody1->GetAngle()-4.7) * 30));
        bulletBody->SetLinearVelocity(force);
        
    }
    else if (playerN == 3 || playerN ==4)
    {
        //create bullet
        CCSprite* bullet = CCSprite::create("laserGreen14.png");
        bullet->setPosition(CCPoint((shipBody2->GetPosition().x + cos(shipBody2->GetAngle()-4.7)*5) *32,
                                    (shipBody2->GetPosition().y + sin(shipBody2->GetAngle()-4.7)*5) *32));
        bullet->setScale(1);
        bullet->setTag(4);
        worldLayer->addChild(bullet);
        
        //body definition for bullet
        b2BodyDef bulletBodyDef;
        bulletBodyDef.type= b2_dynamicBody;
        bulletBodyDef.userData=bullet;
        bulletBodyDef.position.Set(bullet->getPosition().x/32,bullet->getPosition().y/32);
        
        bulletBody = world->CreateBody(&bulletBodyDef);
        bulletBody->CreateFixture(&bulletFixture);
        bulletBody->IsBullet();
        
        b2Vec2 force = b2Vec2((cos(shipBody2->GetAngle()-4.7) * 30) , (sin(shipBody2->GetAngle()-4.7) * 30));
        bulletBody->ApplyLinearImpulse(force, bulletBody->GetPosition());
        
    }
    
    CCCallFunc* disable = CCCallFunc::create(this, callfunc_selector(HelloWorld::disableFireButton));
    CCCallFunc* enable = CCCallFunc::create(this, callfunc_selector(HelloWorld::enableFireButton));
    CCDelayTime* delay = CCDelayTime::create(3);
    CCSequence* seq = CCSequence::create(disable, delay, enable, NULL);
    this->runAction(seq);
}

void HelloWorld::disableFireButton()
{
    fireButton->setEnabled(false);
}

void HelloWorld::enableFireButton()
{
    fireButton->setEnabled(true);
}

void HelloWorld::someOneGotHit(int victim)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");

    if (victim == 1)
    {
        if (score2 != 0)
        {
            score2--;
        }
        
        CCLOG("Score2 %d",score2);
        switch (score2)
        {
            case 4:
                squareBlue5->setVisible(false);
                break;
            case 3:
                squareBlue4->setVisible(false);
                ship1damage1->setVisible(true);
                break;
            case 2:
                squareBlue3->setVisible(false);
                ship1damage2->setVisible(true);
                break;
            case 1:
                squareBlue2->setVisible(false);
                ship1damage3->setVisible(true);
                break;
            case 0:
                squareBlue1->setVisible(false);
                break;
            default:
                break;
        }
        
    }
    else if (victim == 2)
    {
        if (score1 != 0)
        {
            score1--;
        }
        
        CCLOG("Score1 %d",score1);
        switch (score1)
        {
            case 4:
                squareGreen5->setVisible(false);
                break;
            case 3:
                squareGreen4->setVisible(false);
                ship2damage1->setVisible(true);
                break;
            case 2:
                squareGreen3->setVisible(false);
                ship2damage2->setVisible(true);
                break;
            case 1:
                squareGreen2->setVisible(false);
                ship2damage3->setVisible(true);
                break;
            case 0:
                squareGreen1->setVisible(false);
                break;
            default:
                break;
        }
        
    }
    

}

void HelloWorld::toggleShield(int ship)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldUp.mp3");

    if (ship == 1)
    {
        ship1ShieldBool = true;
        ship1shield->setVisible(true);
        
        if (networkLogic->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            networkLogic->sendEvent(5, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(7);
        CCCallFunc* offShield = CCCallFunc::create(this, callfunc_selector(HelloWorld::disableShip1Shield));
        CCSequence* seq = CCSequence::create(delay, offShield, NULL);
        this->runAction(seq);
    }
    else if (ship == 2)
    {
        ship2ShieldBool = true;
        ship2shield->setVisible(true);
        
        if (networkLogic->playerNr == 1)
        {
            ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
            eventContent->put<int, float>(1, ship);
            
            networkLogic->sendEvent(5, eventContent);
        }
        
        CCDelayTime* delay = CCDelayTime::create(7);
        CCCallFunc* offShield = CCCallFunc::create(this, callfunc_selector(HelloWorld::disableShip2Shield));
        CCSequence* seq = CCSequence::create(delay, offShield, NULL);
        this->runAction(seq);
    }
    
}

void HelloWorld::disableShip1Shield()
{
    if (ship1ShieldBool)
    {
        ship1ShieldBool = false;
        ship1shield->setVisible(false);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}

void HelloWorld::disableShip2Shield()
{
    if (ship2ShieldBool)
    {
        ship2ShieldBool = false;
        ship2shield->setVisible(false);
        
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_shieldDown.mp3");
    }
}


void HelloWorld::setViewPointCenter(CCPoint position)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    
    //Edit according to map size
    x = MIN(x, 3072 - winSize.width/2);
    y = MIN(y, 3072 - winSize.height/2);
    CCPoint actualPosition = CCPoint(x, y);
    
    CCPoint centerOfView = CCPoint(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    worldLayer->setPosition(viewPoint);
}

void HelloWorld::fireButtonCall(CCObject *sender)
{
    CCLOG("Fire Button");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser1.mp3");
    
    if (networkLogic->playerNr)
    {
        this->shoot(networkLogic->playerNr);

        CCLOG("Sending from %d", networkLogic->playerNr);
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, bulletBody->GetPosition().x);
        eventContent->put<int, float>(2, bulletBody->GetPosition().y);
        eventContent->put<int, float>(3, bulletBody->GetAngle());
        CCLOG("Bullet X: %f, Y:%f, Angle:%f", bulletBody->GetPosition().x, bulletBody->GetPosition().y, bulletBody->GetAngle());
        
        networkLogic->sendEvent(2, eventContent);
    }
}

void HelloWorld::turnButtonCall(CCObject *sender)
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

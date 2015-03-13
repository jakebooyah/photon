#include "HelloWorldScene.h"

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
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
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
    
    
    //ship shape definition
    b2CircleShape shipShape;
    shipShape.m_p.Set(0, 0);
    shipShape.m_radius = 90/32;
    
    //ship fixture definition
    b2FixtureDef shipFixture;
    shipFixture.density=10;
    shipFixture.friction=0.3;
    shipFixture.restitution=0.9;
    shipFixture.shape=&shipShape;
    
    //create ship 1
    ship1 = CCSprite::create("playerShip2_blue.png");
    ship1->setPosition(CCPoint(512,512));
    ship1->setScale(2);
    ship1->setTag(1);
    worldLayer->addChild(ship1);
    
    //body definition for ship 1
    b2BodyDef shipBodyDef1;
    shipBodyDef1.type= b2_dynamicBody;
    shipBodyDef1.userData=ship1;
    shipBodyDef1.position.Set(ship1->getPosition().x/32,ship1->getPosition().y/32);
    
    shipBody1 = world->CreateBody(&shipBodyDef1);
    shipBody1->CreateFixture(&shipFixture);
    shipBody1->SetGravityScale(10);
    shipBody1->SetAngularDamping(0.3);
    shipBody1->SetTransform(shipBody1->GetPosition(), CC_DEGREES_TO_RADIANS(-45));
    
    //create ship 2
    ship2 = CCSprite::create("playerShip2_green.png");
    ship2->setPosition(CCPoint(2584, 2584));
    ship2->setScale(2);
    ship2->setTag(2);
    worldLayer->addChild(ship2);
    
    //body definition for ship 2
    b2BodyDef shipBodyDef;
    shipBodyDef.type= b2_dynamicBody;
    shipBodyDef.userData=ship2;
    shipBodyDef.position.Set(ship2->getPosition().x/32,ship2->getPosition().y/32);
    
    shipBody2 = world->CreateBody(&shipBodyDef);
    shipBody2->CreateFixture(&shipFixture);
    shipBody2->SetGravityScale(10);
    shipBody2->SetAngularDamping(0.3);
    shipBody2->SetTransform(shipBody2->GetPosition(), CC_DEGREES_TO_RADIANS(135));
    
    
    //Set default view to centre
    CCPoint viewPoint = ccpSub(CCPoint(visibleSize.width/2, visibleSize.height/2), CCPoint(1548, 1548));
    worldLayer->setPosition(viewPoint);
    
    this->addChild(worldLayer);
    
    
    
    
    hudLayer = CCLayer::create();
    
    
    CCSprite* fireButtonS = CCSprite::create("redButton.png");
    CCSprite* fireButtonPressedS = CCSprite::create("redButtonPressed.png");
    
    CCMenuItemSprite* fireButton = CCMenuItemSprite::create(fireButtonS, fireButtonPressedS, this, menu_selector(HelloWorld::fireButtonCall));
    CCMenu* menuFire = CCMenu::create(fireButton, NULL);
    
    menuFire->setPosition(CCPoint(visibleSize.width - 250, 250));
    hudLayer->addChild(menuFire);
    
    
    CCSprite* turnButtonS = CCSprite::create("blueButton.png");
    CCSprite* turnButtonPressedS = CCSprite::create("blueButtonPressed.png");
    
    CCMenuItemSprite* turnButton = CCMenuItemSprite::create(turnButtonS, turnButtonPressedS, this, menu_selector(HelloWorld::turnButtonCall));
    CCMenu* menuTurn = CCMenu::create(turnButton, NULL);
    
    menuTurn->setPosition(CCPoint(250, 250));
    hudLayer->addChild(menuTurn);
    
    scorelabel1 = CCLabelTTF::create("0", "Minecraftia.ttf", 70);
    scorelabel1->setPosition(CCPoint(visibleSize.width/2 - 200, visibleSize.height - 200));
    hudLayer->addChild(scorelabel1);

    scorelabel2 = CCLabelTTF::create("0", "Minecraftia.ttf", 70);
    scorelabel2->setPosition(CCPoint(visibleSize.width/2 + 200, visibleSize.height - 200));
    hudLayer->addChild(scorelabel2);
    
    this->addChild(hudLayer);
    
    
    
    //Contact Listener Init
    _contactListener = new ContactListener();
    world->SetContactListener(_contactListener);
    
    score1 = 0;
    score2 = 0;
    
    scheduleUpdate();
        
    return true;
}

void HelloWorld::update(float delta)
{
    networkLogic->run();
    
    switch (networkLogic->getState())
    {
        case STATE_CONNECTED:
        case STATE_LEFT:
            if (networkLogic->isRoomExists())
            {
                CCLOG("Join");
                networkLogic->setLastInput(INPUT_2);
            } else
            {
                CCLOG("Create");
                networkLogic->setLastInput(INPUT_1);
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
    
    while (!networkLogic->eventQueue.empty())
    {
        std::vector<float> arr = networkLogic->eventQueue.front();
        networkLogic->eventQueue.pop();
        
        int code = arr.back();
        arr.pop_back();

        switch (code)
        {
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
                    CCLOG("Updating.. Player: %d", playerNr);
                    
                    //Correction of Ship 1 with dead reckoning
                    
                    b2Vec2 velocity = shipBody1->GetLinearVelocity();
                    int delay = networkLogic->getRoundTripTime()/100 / 2;
                    
                    b2Vec2 futurePosition = b2Vec2(x1 + velocity.x * delay, y1 + velocity.y * delay);
                    
                    shipBody1->SetTransform(futurePosition, angle1);
                    
                    //Correction of Ship 2 with dead reckoning
                    
                    velocity = shipBody2->GetLinearVelocity();
                    delay = networkLogic->getRoundTripTime()/100 / 2;
                    
                    futurePosition = b2Vec2(x2 + velocity.x * delay, y2 + velocity.y * delay);
                    
                    shipBody2->SetTransform(futurePosition, angle2);
                }
                
                this->turn(playerNr);
                
                break;
            }
                
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
                    
                    b2Vec2 velocity = bulletBody->GetLinearVelocity();
                    int delay = networkLogic->getRoundTripTime()/100 / 2;
                    
                    b2Vec2 futurePosition = b2Vec2(x + velocity.x * delay, y + velocity.y * delay);
                    
                    bulletBody->SetTransform(futurePosition, angle);
                }
                
                break;
            }
                
            case 3:
            {
                int victim = arr.back();
                arr.pop_back();
                
                int playerNr = arr.back();
                arr.pop_back();
                
                this->someOneGotHit(victim);
                
                break;
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
    for(pos = _contactListener->_contacts.begin();
        pos != _contactListener->_contacts.end(); ++pos)
    {
        MyContact contact = *pos;
        
        b2Body *bodyA = contact.fixtureA->GetBody();
        b2Body *bodyB = contact.fixtureB->GetBody();
        if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL)
        {
            CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
            CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
            
            // Sprite A = Ship2, Sprite B = Bullet1
            if (spriteA->getTag() == 2 && spriteB->getTag() == 3)
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                    CCLOG("Ship 2 has been hit");

                    if (networkLogic->playerNr == 1)
                    {
                        someOneGotHit(2);

                        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                        eventContent->put<int, float>(1, 2);
                        
                        networkLogic->sendEvent(3, eventContent);
                    }

                }
            }
            // Sprite A = Bullet1, Sprite B = Ship2
            else if (spriteA->getTag() == 3 && spriteB->getTag() == 2)
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                    CCLOG("Ship 2 has been hit");

                    if (networkLogic->playerNr == 1)
                    {
                        someOneGotHit(2);
                        
                        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                        eventContent->put<int, float>(1, 2);
                        
                        networkLogic->sendEvent(3, eventContent);
                    }

                }
            }
            
            // Sprite A = Ship1, Sprite B = Bullet2
            if (spriteA->getTag() == 1 && spriteB->getTag() == 4)
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                    CCLOG("Ship 1 has been hit");

                    if (networkLogic->playerNr == 1)
                    {
                        someOneGotHit(1);
                        
                        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                        eventContent->put<int, float>(1, 1);
                        
                        networkLogic->sendEvent(3, eventContent);
                    }

                }
            }
            // Sprite A = Bullet2, Sprite B = Ship1
            else if (spriteA->getTag() == 4 && spriteB->getTag() == 1)
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                    CCLOG("Ship 1 has been hit");

                    if (networkLogic->playerNr == 1)
                    {
                        someOneGotHit(1);
                        
                        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
                        eventContent->put<int, float>(1, 1);
                        
                        networkLogic->sendEvent(3, eventContent);
                    }

                }
            }
            
            // Sprite A = Bullet1, Sprite B = Wall
            else if (spriteA->getTag() == 3 && spriteB->getTag() == 5)
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                }
            }
            // Sprite A = Wall, Sprite B = Bullet1
            else if (spriteA->getTag() == 5 && spriteB->getTag() == 3)
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyB);
                }
            }
            
            // Sprite A = Bullet2, Sprite B = Wall
            else if (spriteA->getTag() == 4 && spriteB->getTag() == 5)
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                {
                    toDestroy.push_back(bodyA);
                }
            }
            // Sprite A = Wall, Sprite B = Bullet2
            else if (spriteA->getTag() == 5 && spriteB->getTag() == 4)
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
        world->DestroyBody(body);
    }
    
    world->ClearForces();
    world->DrawDebugData();
}

void HelloWorld::turn(int playerN)
{
    if (playerN == 1)
    {
        CCLOG("Player 1 Turning Left");
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 5) , (sin(shipBody1->GetAngle()-4.7) * 5));

        shipBody1->SetLinearVelocity(force);
        shipBody1->SetAngularVelocity(0.3);
    }
    else if (playerN == 2)
    {
        CCLOG("Player 2 Turning Right");
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 5) , (sin(shipBody1->GetAngle()-4.7) * 5));

        shipBody1->SetLinearVelocity(force);
        shipBody1->SetAngularVelocity(-0.3);
    }
    else if (playerN == 3)
    {
        CCLOG("Player 3 Turning Right");
        
        b2Vec2 force = b2Vec2((cos(shipBody2->GetAngle()-4.7) * 5) , (sin(shipBody2->GetAngle()-4.7) * 5));
        
        shipBody2->SetLinearVelocity(force);
        shipBody2->SetAngularVelocity(0.3);
    }
    else if (playerN == 4)
    {
        CCLOG("Player 4 Turning Right");
        
        b2Vec2 force = b2Vec2((cos(shipBody2->GetAngle()-4.7) * 5) , (sin(shipBody2->GetAngle()-4.7) * 5));
        
        shipBody2->SetLinearVelocity(force);
        shipBody2->SetAngularVelocity(-0.3);
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
    bulletFixture.density=1;
    bulletFixture.friction=0.5;
    bulletFixture.restitution=1;
    bulletFixture.isSensor=false;
    bulletFixture.shape=&bulletShape;
    
    if (playerN == 1 || playerN == 2)
    {
        //create bullet
        bullet = CCSprite::create("laserBlue08.png");
        bullet->setPosition(CCPoint((shipBody1->GetPosition().x + cos(shipBody1->GetAngle()-4.7)*3) *32,
                                    (shipBody1->GetPosition().y + sin(shipBody1->GetAngle()-4.7)*3) *32));
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
        
        b2Vec2 force = b2Vec2((cos(shipBody1->GetAngle()-4.7) * 100) , (sin(shipBody1->GetAngle()-4.7) * 100));
        bulletBody->SetLinearVelocity(force);
    }
    else if (playerN == 3 || playerN ==4)
    {
        //create bullet
        bullet = CCSprite::create("laserGreen14.png");
        bullet->setPosition(CCPoint((shipBody2->GetPosition().x + cos(shipBody1->GetAngle()-4.7)*3) *32,
                                    (shipBody2->GetPosition().y + sin(shipBody1->GetAngle()-4.7)*3) *32));
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
        bulletBody->SetGravityScale(1);
        bulletBody->IsBullet();
        
        b2Vec2 force = b2Vec2((cos(shipBody2->GetAngle()-4.7) * 100) , (sin(shipBody2->GetAngle()-4.7) * 100));
        bulletBody->ApplyLinearImpulse(force, bulletBody->GetPosition());
    }
}

void HelloWorld::someOneGotHit(int victim)
{
    if (victim == 1)
    {
        score2++;
        
        std::stringstream ss;
        ss << score2;
        std::string tmp = ss.str();
        
        scorelabel2->setString(tmp.c_str());

    }
    else if (victim == 2)
    {
        score1++;

        std::stringstream ss;
        ss << score1;
        std::string tmp = ss.str();
        
        scorelabel1->setString(tmp.c_str());
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
    
    this->shoot(networkLogic->playerNr);
        
    if (networkLogic->playerNr)
    {
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
    
    if (networkLogic->playerNr)
    {
        this->turn(networkLogic->playerNr);
        
        CCLOG("Sending from %d", networkLogic->playerNr);
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, shipBody1->GetPosition().x);
        eventContent->put<int, float>(2, shipBody1->GetPosition().y);
        eventContent->put<int, float>(3, shipBody1->GetAngle());
        
        CCLOG("Ship1 X: %f, Y:%f, Angle:%f", shipBody1->GetPosition().x, shipBody1->GetPosition().y, shipBody1->GetAngle());
        
        eventContent->put<int, float>(4, shipBody2->GetPosition().x);
        eventContent->put<int, float>(5, shipBody2->GetPosition().y);
        eventContent->put<int, float>(6, shipBody2->GetAngle());
        
        CCLOG("Ship2 X: %f, Y:%f, Angle:%f", shipBody2->GetPosition().x, shipBody2->GetPosition().y, shipBody2->GetAngle());
        
        networkLogic->sendEvent(1, eventContent);
    }
}

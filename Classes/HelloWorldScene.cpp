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
    
    
    //background init
    CCSprite *background = CCSprite::create("bg.png");
    background->setAnchorPoint(CCPoint(0, 0));
    background->setScale(1.5);
    this->addChild(background);
    
    
    //create wall
    CCSprite* wallCeiling = CCSprite::create("boundary.png");
    wallCeiling->setPosition(CCPoint(1536, 3072));
    wallCeiling->setScale(1.5);
    this->addChild(wallCeiling);
    
    CCSprite* wallGround = CCSprite::create("boundary.png");
    wallGround->setPosition(CCPoint(1536, 0));
    wallGround->setScale(1.5);
    this->addChild(wallGround);
    
    CCSprite* wallLeft = CCSprite::create("boundaryLR.png");
    wallLeft->setPosition(CCPoint(0, 1536));
    wallLeft->setScale(1.5);
    this->addChild(wallLeft);
    
    CCSprite* wallRight = CCSprite::create("boundaryLR.png");
    wallRight->setPosition(CCPoint(3072, 1536));
    wallRight->setScale(1.5);
    this->addChild(wallRight);
    
    
    
    //shape definition
    b2PolygonShape wallUDShape;
    wallUDShape.SetAsBox(3072/32, 96/32);
    
    b2FixtureDef wallUDFixtureDef;
    wallUDFixtureDef.shape = &wallUDShape;
    
    b2PolygonShape wallLRShape;
    wallLRShape.SetAsBox(96/32, 3072/32);
    
    b2FixtureDef wallLRFixtureDef;
    wallLRFixtureDef.shape = &wallLRShape;
    
    
    
    //body definition for wallCeling
    b2BodyDef wallUBodyDef;
    wallUBodyDef.type= b2_staticBody;
    wallUBodyDef.position.Set(wallCeiling->getPosition().x/32,wallCeiling->getPosition().y/32);
    
    b2Body* wallUBody = world->CreateBody(&wallUBodyDef);
    wallUBody->CreateFixture(&wallUDFixtureDef);
    wallUBody->SetGravityScale(10);
    
    

    //body definition for wallGround
    b2BodyDef wallDBodyDef;
    wallDBodyDef.type= b2_staticBody;
    wallDBodyDef.position.Set(wallGround->getPosition().x/32,wallGround->getPosition().y/32);
    
    b2Body* wallDBody = world->CreateBody(&wallDBodyDef);
    wallDBody->CreateFixture(&wallUDFixtureDef);
    wallDBody->SetGravityScale(10);
    
    
    
    //body definition for wallLeft
    b2BodyDef wallLBodyDef;
    wallLBodyDef.type= b2_staticBody;
    wallLBodyDef.position.Set(wallLeft->getPosition().x/32,wallLeft->getPosition().y/32);
    
    b2Body* wallLBody = world->CreateBody(&wallLBodyDef);
    wallLBody->CreateFixture(&wallLRFixtureDef);
    wallLBody->SetGravityScale(10);

    
    
    //body definition for wallRight
    b2BodyDef wallRBodyDef;
    wallRBodyDef.type= b2_staticBody;
    wallRBodyDef.position.Set(wallRight->getPosition().x/32,wallRight->getPosition().y/32);
    
    b2Body* wallRBody = world->CreateBody(&wallRBodyDef);
    wallRBody->CreateFixture(&wallLRFixtureDef);
    wallRBody->SetGravityScale(10);
    
    
    //ship shape definition
    b2CircleShape shipShape;
    shipShape.m_p.Set(0, 0);
    shipShape.m_radius = 75/32;
    
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
    this->addChild(ship1);
    
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
    this->addChild(ship2);
    
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
    this->setPosition(viewPoint);
    
    scheduleUpdate();
    
    this->setTouchEnabled(true);
    
    return true;
}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
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
    this->setPosition(viewPoint);
}

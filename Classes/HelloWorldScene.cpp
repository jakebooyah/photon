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

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    networkLogic = new NetworkLogic();
    
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
    if (networkLogic->playerNr) {
        this->addParticle(networkLogic->playerNr, touch->getLocation().x, touch->getLocation().y);
        
        ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
        eventContent->put<int, float>(1, touch->getLocation().x);
        eventContent->put<int, float>(2, touch->getLocation().y);
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
                CCLOG("PlayerNR = %d", networkLogic->playerNr);
            } else
            {
                CCLOG("Create");
                networkLogic->setLastInput(INPUT_1);
                CCLOG("PlayerNR = %d", networkLogic->playerNr);
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
        CCLOG("adding particles");
        std::vector<float> arr = networkLogic->eventQueue.front();
        networkLogic->eventQueue.pop();

        int playerNr = static_cast<int>(arr.back());
        arr.pop_back();
        
        float x = arr.back();
        arr.pop_back();

        float y = arr.back();

        CCLOG("%d, %f, %f", playerNr, x, y);
        
        this->addParticle(playerNr, x, y);
    }
}

void HelloWorld::addParticle(int playerNr, float x, float y)
{
    CCParticleSystem* particle;

    switch (playerNr) {
        case 1:
            particle = CCParticleFire::create();
            break;
        case 2:
            particle = CCParticleSmoke::create();
            break;
        case 3:
            particle = CCParticleFlower::create();
            break;
        default:
            particle = CCParticleSun::create();
            break;
    }
    particle->setDuration(0.1);
    particle->setSpeed(500);
    particle->setPosition(CCPoint(x,y));
    this->addChild(particle);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

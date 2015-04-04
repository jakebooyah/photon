//
//  CreateRoomScene.cpp
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#include "CreateRoomScene.h"
#include "GameScene.h"
#include "NetworkEngine.h"
#include "MainMenuScene.h"

#include "SimpleAudioEngine.h"
#include "CCScale9Sprite.h"


using namespace cocos2d;

CCScene* CreateRoomScene::scene(int gameMode)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CreateRoomScene *layer = CreateRoomScene::createWithGameMode(gameMode);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

CreateRoomScene* CreateRoomScene::createWithGameMode(int gameMode)
{
    CreateRoomScene* createRoomSceneLayer = new CreateRoomScene();
    if (createRoomSceneLayer && createRoomSceneLayer->initWithGameMode(gameMode))
    {
        createRoomSceneLayer->autorelease();
        return createRoomSceneLayer;
    }
    else
    {
        delete createRoomSceneLayer;
        createRoomSceneLayer = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool CreateRoomScene::initWithGameMode(int gameMode)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    thisGameMode = gameMode;
    
    srand(GETUPTIMEMS());
    
    string roomID = "Room ID: ";
    randomRoomID = rand() % 9999;
    
    NetworkEngine::getInstance()->setRoomID(randomRoomID);
    
    std::stringstream tmp;
    tmp << randomRoomID;
    std::string randomRoomIDString = tmp.str();
    
    roomID.append(randomRoomIDString);
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite *background = CCSprite::create("background.png");
    background->setAnchorPoint(CCPoint(0.5, 0.5));
    background->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    background->setScale(2);
    this->addChild(background);
    
    CCLabelTTF* createRoomLabel = CCLabelTTF::create("Room Created", "Kenvector Future.ttf", 80);
    createRoomLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 200));
    createRoomLabel->setColor(ccWHITE);
    this->addChild(createRoomLabel);
    
    CCLabelTTF* roomIDInfo = CCLabelTTF::create(roomID.c_str(), "Kenvector Future.ttf", 60);
    roomIDInfo->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    roomIDInfo->setColor(ccWHITE);
    this->addChild(roomIDInfo);
    
    cocos2d::extension::CCScale9Sprite* buttonS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonS->setContentSize(CCSize(800, 150));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonPressedS->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* menuButton = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(CreateRoomScene::startGame));
    CCMenu* menuStart = CCMenu::create(menuButton, NULL);
    
    menuStart->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 - 200));
    this->addChild(menuStart);
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create("PLAY", "Kenvector Future.ttf", 60);
    buttonLabel->setColor(ccWHITE);
    buttonLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 - 210));
    this->addChild(buttonLabel);
    
    CCSprite* closeButtonS = CCSprite::create("closeButton.png");
    CCMenuItemSprite* closeButton = CCMenuItemSprite::create(closeButtonS, closeButtonS, this, menu_selector(CreateRoomScene::goToMain));
    CCMenu* menuClose = CCMenu::create(closeButton, NULL);
    menuClose->setPosition(CCPoint(150, visibleSize.height - 150));
    this->addChild(menuClose);
            
    scheduleUpdate();

    return true;
}

void CreateRoomScene::update(float delta)
{
    NetworkEngine::getInstance()->run();
    
    switch (NetworkEngine::getInstance()->getState())
    {
        case STATE_ROOMFULL:
            break;
        case STATE_CONNECTED:
        case STATE_LEFT:
        {
            CCLOG("Create");
            NetworkEngine::getInstance()->setLastInput(INPUT_1);
        }
            break;
        case STATE_DISCONNECTED:
            NetworkEngine::getInstance()->connect();
            break;
        case STATE_CONNECTING:
        case STATE_JOINING:
        case STATE_JOINED:
            break;
        case STATE_LEAVING:
        case STATE_DISCONNECTING:
        default:
            break;
    }
}

void CreateRoomScene::startGame()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
    if (NetworkEngine::getInstance()->getState() == STATE_JOINED)
    {
        if (thisGameMode == 2)
        {
            CCTransitionFade* pScene = CCTransitionFade::create(0.5, GameScene::scene(2), ccBLACK);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
        else if (thisGameMode == 4)
        {
            CCTransitionFade* pScene = CCTransitionFade::create(0.5, GameScene::scene(4), ccBLACK);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
    }
}

void CreateRoomScene::goToMain()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.5, MainMenu::scene(), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
//
//  JoinRoomScene.cpp
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#include "JoinRoomScene.h"
#include "MainMenuScene.h"
#include "NetworkEngine.h"
#include "GameScene.h"

#include "CCScale9Sprite.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

CCScene* JoinRoomScene::scene(int gameMode)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    JoinRoomScene *layer = JoinRoomScene::createWithGameMode(gameMode);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

JoinRoomScene* JoinRoomScene::createWithGameMode(int gameMode)
{
    JoinRoomScene* joinRoomSceneLayer = new JoinRoomScene();
    if (joinRoomSceneLayer && joinRoomSceneLayer->initWithGameMode(gameMode))
    {
        joinRoomSceneLayer->autorelease();
        return joinRoomSceneLayer;
    }
    else
    {
        delete joinRoomSceneLayer;
        joinRoomSceneLayer = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool JoinRoomScene::initWithGameMode(int gameMode)
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    thisGameMode = gameMode;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite *background = CCSprite::create("background.png");
    background->setAnchorPoint(CCPoint(0.5, 0.5));
    background->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    background->setScale(2);
    this->addChild(background);
    
    CCLabelTTF* joinRoomLabel = CCLabelTTF::create("Enter Room ID:", "Kenvector Future.ttf", 80);
    joinRoomLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 200));
    joinRoomLabel->setColor(ccWHITE);
    this->addChild(joinRoomLabel);
    
    extension::CCScale9Sprite* editBoxBackground = extension::CCScale9Sprite::create("panel.png");
    inputField = extension::CCEditBox::create(CCSize(800, 150), editBoxBackground);
    inputField->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    inputField->setFontName("Kenvector Future.ttf");
    inputField->setFontSize(60);
    inputField->setFontColor(ccWHITE);
    inputField->setInputMode(extension::kEditBoxInputModeNumeric);
    inputField->setPlaceHolder("   ID");
    inputField->setPlaceholderFontColor(ccWHITE);
    inputField->setMaxLength(4);
    inputField->setReturnType(extension::kKeyboardReturnTypeSend);
    this->addChild(inputField);
    
    extension::CCScale9Sprite* buttonS = extension::CCScale9Sprite::create("panel.png");
    buttonS->setContentSize(CCSize(800, 150));
    
    extension::CCScale9Sprite* buttonPressedS = extension::CCScale9Sprite::create("panel.png");
    buttonPressedS->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* menuButton = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(JoinRoomScene::joinGame));
    CCMenu* menuStart = CCMenu::create(menuButton, NULL);
    
    menuStart->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-200));
    this->addChild(menuStart);
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create("JOIN", "Kenvector Future.ttf", 60);
    buttonLabel->setColor(ccWHITE);
    buttonLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-210));
    this->addChild(buttonLabel);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 01 Atempause.mp3", true);
    
    joinGameIsPressed = false;

    this->scheduleUpdate();
    
    return true;
}

void JoinRoomScene::update(float delta)
{
    NetworkEngine::getInstance()->run();
    
    switch (NetworkEngine::getInstance()->getState())
    {
        case STATE_ROOMFULL:
            break;
        case STATE_CONNECTED:
        case STATE_LEFT:
        {
            if (joinGameIsPressed)
            {
                NetworkEngine::getInstance()->setLastInput(INPUT_2);
            }
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
    
    if (NetworkEngine::getInstance()->getState() == STATE_JOINED)
    {
        if (thisGameMode == 2)
        {
            CCTransitionFade* pScene = CCTransitionFade::create(0.7, GameScene::scene(2), ccBLACK);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
        else if (thisGameMode == 4)
        {
            CCTransitionFade* pScene = CCTransitionFade::create(0.7, GameScene::scene(4), ccBLACK);
            CCDirector::sharedDirector()->replaceScene(pScene);
        }
    }
}

void JoinRoomScene::joinGame()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_lose.mp3");
    
    std::string roomIDString = inputField->getText();
    std::istringstream buffer(roomIDString);
    int roomID;
    buffer >> roomID;
    
    NetworkEngine::getInstance()->setRoomID(roomID);
    
    joinGameIsPressed = true;
}
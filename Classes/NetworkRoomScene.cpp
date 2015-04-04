//
//  NetworkRoomScene.cpp
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#include "NetworkRoomScene.h"
#include "CreateRoomScene.h"
#include "JoinRoomScene.h"
#include "MainMenuScene.h"

#include "CCScale9Sprite.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

CCScene* NetworkRoomScene::scene(int gameMode)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    NetworkRoomScene *layer = NetworkRoomScene::createWithGameMode(gameMode);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

NetworkRoomScene* NetworkRoomScene::createWithGameMode(int gameMode)
{
    NetworkRoomScene* networkRoomSceneLayer = new NetworkRoomScene();
    if (networkRoomSceneLayer && networkRoomSceneLayer->initWithGameMode(gameMode))
    {
        networkRoomSceneLayer->autorelease();
        return networkRoomSceneLayer;
    }
    else
    {
        delete networkRoomSceneLayer;
        networkRoomSceneLayer = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool NetworkRoomScene::initWithGameMode(int gameMode)
{
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
    
    if (gameMode == 2)
    {
        CCLabelTTF* title = CCLabelTTF::create("2PLAY", "Kenvector Future.ttf", 80);
        title->setColor(ccWHITE);
        title->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2+210));
        this->addChild(title);
    }
    else if (gameMode == 4)
    {
        CCLabelTTF* title = CCLabelTTF::create("4PLAY", "Kenvector Future.ttf", 80);
        title->setColor(ccWHITE);
        title->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2+210));
        this->addChild(title);
    }
    
    cocos2d::extension::CCScale9Sprite* buttonS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonS->setContentSize(CCSize(800, 150));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonPressedS->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* startButton1 = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(NetworkRoomScene::goToCreateRoom));
    CCMenu* menuStart1 = CCMenu::create(startButton1, NULL);
    
    menuStart1->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    this->addChild(menuStart1);
    
    CCLabelTTF* buttonLabel1 = CCLabelTTF::create("CREATE", "Kenvector Future.ttf", 60);
    buttonLabel1->setColor(ccWHITE);
    buttonLabel1->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 - 10));
    this->addChild(buttonLabel1);
    
    cocos2d::extension::CCScale9Sprite* buttonS1 = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonS1->setContentSize(CCSize(800, 150));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS1 = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonPressedS1->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* startButton2 = CCMenuItemSprite::create(buttonS1, buttonPressedS1, this, menu_selector(NetworkRoomScene::goToJoinRoom));
    CCMenu* menuStart2 = CCMenu::create(startButton2, NULL);
    
    menuStart2->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2- 200));
    this->addChild(menuStart2);
    
    CCLabelTTF* buttonLabel2 = CCLabelTTF::create("JOIN", "Kenvector Future.ttf", 60);
    buttonLabel2->setColor(ccWHITE);
    buttonLabel2->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2- 210));
    this->addChild(buttonLabel2);
    
    CCSprite* closeButtonS = CCSprite::create("closeButton.png");
    CCMenuItemSprite* closeButton = CCMenuItemSprite::create(closeButtonS, closeButtonS, this, menu_selector(NetworkRoomScene::goToMain));
    CCMenu* menuClose = CCMenu::create(closeButton, NULL);
    menuClose->setPosition(CCPoint(visibleSize.width - 150, visibleSize.height - 150));
    this->addChild(menuClose);
    
    return true;
}

void NetworkRoomScene::goToCreateRoom()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.5, CreateRoomScene::scene(thisGameMode), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void NetworkRoomScene::goToJoinRoom()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.5, JoinRoomScene::scene(thisGameMode), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}

void NetworkRoomScene::goToMain()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.5, MainMenu::scene(), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
//
//  MainMenuScene.cpp
//  photon
//
//  Created by Jake on 3/14/15.
//
//

#include "MainMenuScene.h"
//#include "TwoPlayerGameScene.h"
#include "NetworkRoomScene.h"

#include "CCScale9Sprite.h"
#include "SimpleAudioEngine.h"
#include "NetworkEngine.h"

CCScene* MainMenu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenu *layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    NetworkEngine::getInstance()->setRoomID(0);
    NetworkEngine::getInstance()->setLastInput(INPUT_EXIT);
    NetworkEngine::getInstance()->connect();
    NetworkEngine::getInstance()->run();
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCSprite *background = CCSprite::create("background.png");
    background->setAnchorPoint(CCPoint(0.5, 0.5));
    background->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    background->setScale(2);
    this->addChild(background);
    
    CCLabelTTF* title = CCLabelTTF::create("PHOTON", "Kenvector Future.ttf", 80);
    title->setColor(ccWHITE);
    title->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2+210));
    this->addChild(title);
    
    cocos2d::extension::CCScale9Sprite* buttonS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonS->setContentSize(CCSize(800, 150));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonPressedS->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* startButton1 = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(MainMenu::startGame2));
    CCMenu* menuStart1 = CCMenu::create(startButton1, NULL);
    
    menuStart1->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    this->addChild(menuStart1);
    
    CCLabelTTF* buttonLabel1 = CCLabelTTF::create("2PLAY", "Kenvector Future.ttf", 60);
    buttonLabel1->setColor(ccWHITE);
    buttonLabel1->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-10));
    this->addChild(buttonLabel1);
    
    cocos2d::extension::CCScale9Sprite* buttonS1 = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonS1->setContentSize(CCSize(800, 150));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS1 = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonPressedS1->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* startButton2 = CCMenuItemSprite::create(buttonS1, buttonPressedS1, this, menu_selector(MainMenu::startGame4));
    CCMenu* menuStart2 = CCMenu::create(startButton2, NULL);
    
    menuStart2->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-200));
    this->addChild(menuStart2);
    
    CCLabelTTF* buttonLabel2 = CCLabelTTF::create("4PLAY", "Kenvector Future.ttf", 60);
    buttonLabel2->setColor(ccWHITE);
    buttonLabel2->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-210));
    this->addChild(buttonLabel2);
    
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 15 Another weird ambient tune..mp3", true);
    
    return true;
}

void MainMenu::startGame2()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_lose.mp3");
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.7, NetworkRoomScene::scene(2), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);

}

void MainMenu::startGame4()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_lose.mp3");
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.7, NetworkRoomScene::scene(4), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
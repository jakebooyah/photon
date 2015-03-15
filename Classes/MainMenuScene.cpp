//
//  MainMenuScene.cpp
//  photon
//
//  Created by Jake on 3/14/15.
//
//

#include "MainMenuScene.h"
#include "HelloWorldScene.h"
#include "CCScale9Sprite.h"
#include "SimpleAudioEngine.h"

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
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //background init
    CCSprite *background = CCSprite::create("bg.png");
    background->setAnchorPoint(CCPoint(0, 0));
    background->setScale(1.5);
    this->addChild(background);
    
    CCSprite *title = CCSprite::create("title.png");
    title->setScale(2);
    title->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2+180));
    this->addChild(title);
    
    cocos2d::extension::CCScale9Sprite* buttonS = cocos2d::extension::CCScale9Sprite::create("metalPanel_yellow.png");
    buttonS->setContentSize(CCSize(800, 200));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS = cocos2d::extension::CCScale9Sprite::create("metalPanel_red.png");
    buttonPressedS->setContentSize(CCSize(800, 200));
    
    CCMenuItemSprite* startButton = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(MainMenu::startGame));
    CCMenu* menuStart = CCMenu::create(startButton, NULL);
    
    menuStart->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-280));
    this->addChild(menuStart);
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create("PLAY", "Kenvector Future.ttf", 60);
    buttonLabel->setColor(ccBLACK);
    buttonLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-290));
    this->addChild(buttonLabel);
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.4);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 15 Another weird ambient tune..mp3", true);
    
    return true;
}

void MainMenu::startGame()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser2.mp3");

    CCTransitionFade* pScene = CCTransitionFade::create(0.7,HelloWorld::scene(), ccWHITE);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
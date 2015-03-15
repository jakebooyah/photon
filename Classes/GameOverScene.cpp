//
//  GameOverScene.cpp
//  photon
//
//  Created by Jake on 3/14/15.
//
//

#include "GameOverScene.h"
#include "CCScale9Sprite.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

CCScene* GameOver::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameOver *layer = GameOver::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameOver::init()
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
    
    CCLabelTTF* gameOverLabel = CCLabelTTF::create("GAME OVER", "Kenvector Future.ttf", 80);
    gameOverLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 100));
    this->addChild(gameOverLabel);
    
    cocos2d::extension::CCScale9Sprite* buttonS = cocos2d::extension::CCScale9Sprite::create("metalPanel_yellow.png");
    buttonS->setContentSize(CCSize(800, 200));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS = cocos2d::extension::CCScale9Sprite::create("metalPanel_red.png");
    buttonPressedS->setContentSize(CCSize(800, 200));
    
    CCMenuItemSprite* menuButton = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(GameOver::goToStart));
    CCMenu* menuStart = CCMenu::create(menuButton, NULL);
    
    menuStart->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-100));
    this->addChild(menuStart);
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create("MAIN MENU", "Kenvector Future.ttf", 60);
    buttonLabel->setColor(ccBLACK);
    buttonLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-110));
    this->addChild(buttonLabel);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 01 Atempause.mp3", true);
    
    return true;
}

void GameOver::goToStart()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_laser2.mp3");

    CCTransitionFade* pScene = CCTransitionFade::create(0.7,MainMenu::scene(), ccWHITE);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
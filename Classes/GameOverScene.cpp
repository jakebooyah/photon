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
    
    CCSprite *background = CCSprite::create("background.png");
    background->setAnchorPoint(CCPoint(0.5, 0.5));
    background->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2));
    background->setScale(2);
    this->addChild(background);
    
    CCLabelTTF* gameOverLabel = CCLabelTTF::create("GAME OVER", "Kenvector Future.ttf", 80);
    gameOverLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 100));
    gameOverLabel->setColor(ccWHITE);
    this->addChild(gameOverLabel);
    
    cocos2d::extension::CCScale9Sprite* buttonS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonS->setContentSize(CCSize(800, 150));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonPressedS->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* menuButton = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(GameOver::goToStart));
    CCMenu* menuStart = CCMenu::create(menuButton, NULL);
    
    menuStart->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-100));
    this->addChild(menuStart);
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create("MAIN MENU", "Kenvector Future.ttf", 60);
    buttonLabel->setColor(ccWHITE);
    buttonLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-110));
    this->addChild(buttonLabel);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 01 Atempause.mp3", true);
    
    return true;
}

void GameOver::goToStart()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_lose.mp3");

    CCTransitionFade* pScene = CCTransitionFade::create(0.7,MainMenu::scene(), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
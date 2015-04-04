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
#include "NetworkEngine.h"

using namespace cocos2d;

CCScene* GameOver::scene(int winner)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameOver *layer = GameOver::createWithWinner(winner);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameOver* GameOver::createWithWinner(int winner)
{
    GameOver* gameOverLayer = new GameOver();
    if (gameOverLayer && gameOverLayer->initWithWinner(winner))
    {
        gameOverLayer->autorelease();
        return gameOverLayer;
    }
    else
    {
        delete gameOverLayer;
        gameOverLayer = NULL;
        return NULL;
    }
}

// on "init" you need to initialize your instance
bool GameOver::initWithWinner(int winner)
{
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
    
    if (winner == 1)
    {
        CCSprite* winner = CCSprite::create("ship_blue.png");
        winner->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 30));
        this->addChild(winner);

    }
    else if (winner == 2)
    {
        CCSprite* winner = CCSprite::create("ship_green.png");
        winner->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 30));
        this->addChild(winner);

    }
    else if (winner == 3)
    {
        CCSprite* winner = CCSprite::create("ship_ai.png");
        winner->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 30));
        this->addChild(winner);

    }
    
    CCLabelTTF* winnerLabel = CCLabelTTF::create("Winner", "Kenvector Future.ttf", 80);
    winnerLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2 + 210));
    winnerLabel->setColor(ccWHITE);
    this->addChild(winnerLabel);
    
    cocos2d::extension::CCScale9Sprite* buttonS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonS->setContentSize(CCSize(800, 150));
    
    cocos2d::extension::CCScale9Sprite* buttonPressedS = cocos2d::extension::CCScale9Sprite::create("panel.png");
    buttonPressedS->setContentSize(CCSize(800, 150));
    
    CCMenuItemSprite* menuButton = CCMenuItemSprite::create(buttonS, buttonPressedS, this, menu_selector(GameOver::goToStart));
    CCMenu* menuStart = CCMenu::create(menuButton, NULL);
    
    menuStart->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-200));
    this->addChild(menuStart);
    
    CCLabelTTF* buttonLabel = CCLabelTTF::create("MAIN MENU", "Kenvector Future.ttf", 60);
    buttonLabel->setColor(ccWHITE);
    buttonLabel->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2-210));
    this->addChild(buttonLabel);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("C418 - Seven Years of Server Data - 01 Atempause.mp3", true);
    
    scheduleUpdate();
    
    return true;
}

void GameOver::update(float delta)
{
    NetworkEngine::getInstance()->run();
    
    switch (NetworkEngine::getInstance()->getState())
    {
        case STATE_ROOMFULL:
            break;
        case STATE_CONNECTED:
        case STATE_LEFT:
            break;
        case STATE_DISCONNECTED:
            NetworkEngine::getInstance()->connect();
            break;
        case STATE_CONNECTING:
        case STATE_JOINING:
        case STATE_JOINED:
            NetworkEngine::getInstance()->setLastInput(INPUT_1);
            break;
        case STATE_LEAVING:
        case STATE_DISCONNECTING:
        default:
            break;
    }
}

void GameOver::goToStart()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");

    CCTransitionFade* pScene = CCTransitionFade::create(0.5,MainMenu::scene(), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
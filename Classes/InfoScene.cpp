//
//  InfoScene.cpp
//  photon
//
//  Created by Jake on 4/16/15.
//
//

#include "InfoScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

CCScene* Info::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Info *layer = Info::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Info::init()
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
    
    CCLabelTTF* title = CCLabelTTF::create("PHOTON", "Kenvector Future.ttf", 100);
    title->setColor(ccWHITE);
    title->setPosition(CCPoint(visibleSize.width/2, visibleSize.height/2+210));
    this->addChild(title);

    CCSprite* closeButtonS = CCSprite::create("closeButton.png");
    CCMenuItemSprite* closeButton = CCMenuItemSprite::create(closeButtonS, closeButtonS, this, menu_selector(Info::goToMain));
    CCMenu* menuClose = CCMenu::create(closeButton, NULL);
    menuClose->setPosition(CCPoint(visibleSize.width - 150, visibleSize.height - 150));
    this->addChild(menuClose);
    
    return true;
}

void Info::goToMain()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("sfx_zap.mp3");
    
    CCTransitionFade* pScene = CCTransitionFade::create(0.5, MainMenu::scene(), ccBLACK);
    CCDirector::sharedDirector()->replaceScene(pScene);
}
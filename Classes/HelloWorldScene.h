#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NetworkLogic.h"
#include "Box2d.h"
#include "ContactListener.h"

using namespace cocos2d;

class HelloWorld : public CCLayer, public b2ContactListener
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    float deltaTime;
    
    void setViewPointCenter(CCPoint position);

private:
    virtual void update(float delta);
    
    int score1;
    int score2;
    
    bool ship1ShieldBool;
    bool ship2ShieldBool;
    
    bool Player2Joined;
    bool Player3Joined;
    bool Player4Joined;
    
    void turn(int playerNr);
    void shoot(int playerNr);
    void someOneGotHit(int victim);
    
    void removeLoading();
    
    void disableFireButton();
    void enableFireButton();
    
    void toggleShield(int ship);
    void disableShip1Shield();
    void disableShip2Shield();
    
    void gameOver();
    
    void sendPositions();
    
    CCMenuItemSprite* fireButton;
    
    NetworkLogic* networkLogic;
    
    b2World *world;
    
    ContactListener *_contactListener;
    
    CCLayer *worldLayer;
    CCLayer *hudLayer;
    CCLayer *loadingLayer;
    
    CCSprite *squareBlue1;
    CCSprite *squareBlue2;
    CCSprite *squareBlue3;
    CCSprite *squareBlue4;
    CCSprite *squareBlue5;
    
    CCSprite *squareGreen1;
    CCSprite *squareGreen2;
    CCSprite *squareGreen3;
    CCSprite *squareGreen4;
    CCSprite *squareGreen5;

    CCSprite *ship1;
    CCSprite *ship1damage1;
    CCSprite *ship1damage2;
    CCSprite *ship1damage3;
    CCSprite *ship1shield;

    b2Body *shipBody1;
    
    CCSprite *ship2;
    CCSprite *ship2damage1;
    CCSprite *ship2damage2;
    CCSprite *ship2damage3;
    CCSprite *ship2shield;
    
    b2Body *shipBody2;
    
    b2Body *bulletBody;
    
    void fireButtonCall(CCObject *sender);
    void turnButtonCall(CCObject *sender);
};

#endif // __HELLOWORLD_SCENE_H__

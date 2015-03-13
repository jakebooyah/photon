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
    
    void turn(int playerNr);
    void shoot(int playerNr);
    void someOneGotHit(int victim);
    
    void disableFireButton();
    void enableFireButton();
    
    CCMenuItemSprite* fireButton;
    
    NetworkLogic* networkLogic;
    
    b2World *world;
    
    ContactListener *_contactListener;
    
    CCLayer *worldLayer;
    CCLayer *hudLayer;

    CCLabelTTF *scorelabel1;
    CCLabelTTF *scorelabel2;

    CCSprite *ship1;
    b2Body *shipBody1;
    
    CCSprite *ship2;
    b2Body *shipBody2;
    
    CCSprite *bullet;
    b2Body *bulletBody;
    
    void fireButtonCall(CCObject *sender);
    void turnButtonCall(CCObject *sender);
};

#endif // __HELLOWORLD_SCENE_H__

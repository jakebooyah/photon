#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NetworkLogic.h"
#include "Box2d.h"

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
        
    void turn(int playerNr);
        
    NetworkLogic* networkLogic;
    
    b2World *world;
    
    CCLayer *worldLayer;
    CCLayer *hudLayer;
    
    CCSprite *ship1;
    b2Body *shipBody1;
    
    CCSprite *ship2;
    b2Body *shipBody2;
    
    CCSprite *bullet;
    b2Body *bulletBody;
    
    b2Vec2 prevPosition;
    int pathLength;
    
    void fireButtonCall(CCObject *sender);
    void turnButtonCall(CCObject *sender);
};

#endif // __HELLOWORLD_SCENE_H__

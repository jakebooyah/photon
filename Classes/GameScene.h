//
//  GameScene.h
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#ifndef __photon__GameScene__
#define __photon__GameScene__

#include "cocos2d.h"
#include "NetworkEngine.h"
#include "Box2d.h"
#include "ContactListener.h"
#include "CCScale9Sprite.h"

using namespace cocos2d;

class GameScene : public CCLayer, public b2ContactListener
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithGameMode(int gameMode);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int gameMode);
    
    // implement the "static node()" method manually
    static GameScene* createWithGameMode(int gameMode);
    
    float deltaTime;
    
    void setViewPointCenter(CCPoint position);
    
private:
    virtual void update(float delta);
    
    int thisGameMode;
    
    int score1;
    int score2;
    
    float heatAmount;
    float maxHeatAmount;
        
    bool isGameOver;
    
    bool PlayerAllJoined;
    
    void toggleInvertRole();
    
    bool isRoleFlipped;
    
    void moveSeekingAI();
    
    void hideShip1Flame();
    void hideShip2Flame();
    
    void turn(int playerNr, int direction);
    void shoot(int playerNr);
    void someOneGotHit(int victim);
    
    void removeLoading();
        
    void spawnRunes();
    
    void updateHpBar();
    
    void toggleHPUp(int ship);
    
    bool ship1ShieldBool;
    bool ship2ShieldBool;
    
    void toggleShield(int ship);
    void disableShip1Shield();
    void disableShip2Shield();
    
    bool ship1DoubleDamageBool;
    bool ship2DoubleDamageBool;
    
    void toggleDoubleDamage(int ship);
    void disableShip1DoubleDamage();
    void disableShip2DoubleDamage();
    
    bool ship1InvertDirectionBool;
    bool ship2InvertDirectionBool;
    
    void toggleInvertDirection(int ship);
    void disableShip1InvertDirection();
    void disableShip2InvertDirection();
    
    void gameOver();
    
    void sendPositions();
    
    CCProgressTimer* heatProgress;
        
    CCMenuItemSprite* fireButton;
    CCMenuItemSprite* turnLeftButton;
    CCMenuItemSprite* turnRightButton;
    
    CCLabelTTF* overHeatLabel;
    CCLabelTTF* switchRoleLabel;
    
    b2World *world;
    
    ContactListener *_contactListener;
    
    CCTMXTiledMap *tilemapSpawn1;
    CCTMXLayer *tileMapSpawn1Layer;
    
    CCTMXTiledMap *tilemapSpawn2;
    CCTMXLayer *tileMapSpawn2Layer;
    
    CCTMXTiledMap *statusBlue;
    CCTMXLayer *tileMapStatusBlueLayer;
    
    CCTMXTiledMap *statusGreen;
    CCTMXLayer *tileMapStatusGreenLayer;
    
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
    CCSprite *ship1flame;
    CCSprite *ship1shield;
    
    b2Body *shipBody1;
    
    CCSprite *ship2;
    CCSprite *ship2flame;
    CCSprite *ship2shield;
    
    b2Body *shipBody2;
    
    CCSprite *wormHole;
    b2Body *wormHoleBody;
    
    CCSprite *planet;
    b2Body *planetBody;
    
    b2Body *bulletBody;
    
    void fireButtonCall(CCObject *sender);
    void turnRightButtonCall(CCObject *sender);
    void turnLeftButtonCall(CCObject *sender);

};

#endif /* defined(__photon__GameScene__) */

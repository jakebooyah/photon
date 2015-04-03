////
////  TwoPlayerScene.h
////  photon
////
////  Created by Jake on 3/28/15.
////
////
//
//#ifndef __photon__TwoPlayerGameScene__
//#define __photon__TwoPlayerGameScene__
//
//#include "cocos2d.h"
//#include "NetworkLogic.h"
//#include "Box2d.h"
//#include "ContactListener.h"
//
//using namespace cocos2d;
//
//class TwoPlayerGameScene : public CCLayer, public b2ContactListener
//{
//public:
//    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
//    virtual bool init();
//    
//    // there's no 'id' in cpp, so we recommend returning the class instance pointer
//    static cocos2d::CCScene* scene();
//    
//    // implement the "static node()" method manually
//    CREATE_FUNC(TwoPlayerGameScene);
//    
//    float deltaTime;
//    
//    void setViewPointCenter(CCPoint position);
//    
//private:
//    
//    virtual void update(float delta);
//    
//    int score1;
//    int score2;
//    
//    bool isGameOver;
//    
//    bool Player2Joined;
//    
//    void turn(int playerNr);
//    void shoot(int playerNr);
//    void someOneGotHit(int victim);
//    
//    void removeLoading();
//    
//    void disableFireButton();
//    void enableFireButton();
//    
//    void spawnRunes();
//    
//    void updateHpBar();
//    
//    void hideShip1Flame();
//    
//    void toggleHPUp(int ship);
//    
//    bool ship1ShieldBool;
//    
//    void toggleShield(int ship);
//    void disableShip1Shield();
//    
//    bool ship1DoubleDamageBool;
//    
//    void toggleDoubleDamage(int ship);
//    void disableShip1DoubleDamage();
//    
//    bool ship1InvertRoleBool;
//    
//    void toggleInvertRole(int ship);
//    void disableShip1InvertRole();
//    
//    void gameOver();
//    
//    void sendPositions();
//    
//    void moveSeekingAI();
//    
//    CCMenuItemSprite* fireButton;
//    
//    NetworkLogic* networkLogic;
//    
//    b2World *world;
//    
//    ContactListener *_contactListener;
//    
//    CCTMXTiledMap *tilemapSpawn1;
//    CCTMXLayer *tileMapSpawn1Layer;
//    
//    CCTMXTiledMap *tilemapSpawn2;
//    CCTMXLayer *tileMapSpawn2Layer;
//    
//    CCTMXTiledMap *statusBlue;
//    CCTMXLayer *tileMapStatusBlueLayer;
//    
//    CCTMXTiledMap *statusGreen;
//    CCTMXLayer *tileMapStatusGreenLayer;
//    
//    CCLayer *worldLayer;
//    CCLayer *hudLayer;
//    CCLayer *loadingLayer;
//    
//    CCSprite *squareBlue1;
//    CCSprite *squareBlue2;
//    CCSprite *squareBlue3;
//    CCSprite *squareBlue4;
//    CCSprite *squareBlue5;
//    
//    CCSprite *squareGreen1;
//    CCSprite *squareGreen2;
//    CCSprite *squareGreen3;
//    CCSprite *squareGreen4;
//    CCSprite *squareGreen5;
//    
//    CCSprite *ship1;
//    CCSprite *ship1flame;
//    CCSprite *ship1shield;
//    
//    b2Body *shipBody1;
//    
//    CCSprite *ship2;
//    
//    b2Body *shipBody2;
//    
//    CCSprite *planet;
//    b2Body *planetBody;
//    
//    b2Body *bulletBody;
//    
//    void fireButtonCall(CCObject *sender);
//    void turnButtonCall(CCObject *sender);
//};
//
//#endif /* defined(__photon__TwoPlayerGameScene__) */

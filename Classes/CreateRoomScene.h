//
//  CreateRoomScene.h
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#ifndef __photon__CreateRoomScene__
#define __photon__CreateRoomScene__

#include "cocos2d.h"

using namespace cocos2d;

class CreateRoomScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithGameMode(int gameMode);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int gameMode);
    
    // implement the "static node()" method manually
    static CreateRoomScene* createWithGameMode(int gameMode);
    
private:
    int thisGameMode;
    void startGame();
    void goToMain();
    
    int randomRoomID;
    
    virtual void update(float delta);
};

#endif /* defined(__photon__CreateRoomScene__) */

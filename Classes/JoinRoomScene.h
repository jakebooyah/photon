//
//  JoinRoomScene.h
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#ifndef __photon__JoinRoomScene__
#define __photon__JoinRoomScene__

#include "cocos2d.h"
#include "CCEditBox.h"

using namespace cocos2d;

class JoinRoomScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithGameMode(int gameMode);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int gameMode);
    
    // implement the "static node()" method manually
    static JoinRoomScene* createWithGameMode(int gameMode);
    
private:
    int thisGameMode;
    bool joinGameIsPressed;
    void joinGame();
    void goToMain();
    
    extension::CCEditBox* inputField;
    
    virtual void update(float delta);

};

#endif /* defined(__photon__JoinRoomScene__) */

//
//  NetworkRoomScene.h
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#ifndef __photon__NetworkRoomScene__
#define __photon__NetworkRoomScene__

#include "cocos2d.h"

using namespace cocos2d;

class NetworkRoomScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithGameMode(int gameMode);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int gameMode);
    
    // implement the "static node()" method manually
    static NetworkRoomScene* createWithGameMode(int gameMode);
    
private:
    int thisGameMode;
    void goToCreateRoom();
    void goToJoinRoom();
    void goToMain();
};

#endif /* defined(__photon__NetworkRoomScene__) */

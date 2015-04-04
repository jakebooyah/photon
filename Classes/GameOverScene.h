//
//  GameOverScene.h
//  photon
//
//  Created by Jake on 3/14/15.
//
//

#ifndef __photon__GameOverScene__
#define __photon__GameOverScene__

#include "cocos2d.h"

using namespace cocos2d;

class GameOver : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool initWithWinner(int winner);
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(int winner);
    
    // implement the "static node()" method manually
    static GameOver* createWithWinner(int winner);
    
private:    
    virtual void update(float delta);

    void goToStart();
};
#endif /* defined(__photon__GameOverScene__) */

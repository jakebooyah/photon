//
//  MainMenuScene.h
//  photon
//
//  Created by Jake on 3/14/15.
//
//

#ifndef __photon__MainMenuScene__
#define __photon__MainMenuScene__

#include "cocos2d.h"

using namespace cocos2d;

class MainMenu : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(MainMenu);
    
private:
    void startGame2();
    void startGame4();
};

#endif /* defined(__photon__MainMenuScene__) */

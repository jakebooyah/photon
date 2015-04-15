//
//  InfoScene.h
//  photon
//
//  Created by Jake on 4/16/15.
//
//

#ifndef __photon__InfoScene__
#define __photon__InfoScene__

#include "cocos2d.h"

using namespace cocos2d;

class Info : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(Info);
    
private:
    void goToMain();
};
#endif /* defined(__photon__InfoScene__) */

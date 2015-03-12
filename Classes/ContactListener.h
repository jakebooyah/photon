//
//  ContactListener.h
//  photon
//
//  Created by Jake on 3/13/15.
//
//

#ifndef __photon__ContactListener__
#define __photon__ContactListener__

#include <stdio.h>
#include "Box2D.h"
#include <vector>

struct MyContact
{
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const MyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
};


class ContactListener : public b2ContactListener
{
public:
    std::vector<MyContact> _contacts;
    
    ContactListener();
    ~ContactListener();
    
    virtual void BeginContact(b2Contact* contact);
    virtual void EndContact(b2Contact* contact);
    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    
private:

};
#endif /* defined(__photon__ContactListener__) */

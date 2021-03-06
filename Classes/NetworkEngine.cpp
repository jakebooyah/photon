//
//  NetworkEngine.cpp
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#include "NetworkEngine.h"
#include "cocos2d.h"

static const ExitGames::Common::JString appId = L"78de70d1-ee79-46df-bc56-a296ef1a4535"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";

static const bool autoLobbbyStats = true;
static const bool useDefaultRegion = false;

NetworkEngine* NetworkEngine::instance = NULL;

static const ExitGames::Common::JString PLAYER_NAME =
#if defined _EG_MARMALADE_PLATFORM
#	if defined I3D_ARCH_X86
#		if defined _EG_MS_COMPILER
L"Marmalade X86 Windows";
#		else
L"Marmalade X86 OS X";
#		endif
#	elif defined I3D_ARCH_ARM
L"Marmalade ARM";
#	elif defined I3D_ARCH_MIPS
L"Marmalade MIPS";
#	else
L"unknown Marmalade platform";
#	endif
#elif defined _EG_WINDOWS_PLATFORM
L"Windows";
#elif defined _EG_IPHONE_PLATFORM
L"iOS";
#elif defined _EG_IMAC_PLATFORM
L"OS X";
#elif defined _EG_ANDROID_PLATFORM
L"Android";
#elif defined _EG_BLACKBERRY_PLATFORM
L"Blackberry";
#elif defined _EG_PS3_PLATFORM
L"PS3";
#elif defined _EG_LINUX_PLATFORM
L"Linux";
#else
L"unknown platform";
#endif

ExitGames::Common::JString& NetworkEngineListener::toString(ExitGames::Common::JString& retStr, bool /*withTypes*/) const
{
    return retStr;
}

State StateAccessor::getState(void) const
{
    return mState;
}

void StateAccessor::setState(State newState)
{
    mState = newState;
    for(unsigned int i=0; i<mStateUpdateListeners.getSize(); i++)
        mStateUpdateListeners[i]->stateUpdate(newState);
}

void StateAccessor::registerForStateUpdates(NetworkEngineListener* listener)
{
    mStateUpdateListeners.addElement(listener);
}

Input NetworkEngine::getLastInput(void) const
{
    return mLastInput;
}

void NetworkEngine::setLastInput(Input newInput)
{
    mLastInput = newInput;
}

State NetworkEngine::getState(void) const
{
    return mStateAccessor.getState();
}

// functions
NetworkEngine::NetworkEngine(const ExitGames::LoadBalancing::AuthenticationValues& authenticationValues)
#ifdef _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
: mLoadBalancingClient(*this, appId, appVersion, PLAYER_NAME+GETTIMEMS(), false, authenticationValues, autoLobbbyStats, useDefaultRegion)
, mLastInput(INPUT_NON)
, mLastActorNr(0)
, playerNr(0)
, roomID(0)
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
{
    mStateAccessor.setState(STATE_INITIALIZED);
    mLoadBalancingClient.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // that instance of LoadBalancingClient and its implementation details
    mLogger.setListener(*this);
    mLogger.setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // this class
    ExitGames::Common::Base::setListener(this);
    ExitGames::Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS)); // all classes that inherit from Base
}

NetworkEngine* NetworkEngine::getInstance()
{
    //If the singleton has no instance yet, create one
    if(NULL == instance)
    {
        //Create an instance to the singleton
        instance = new NetworkEngine();
    }
    
    //Return the singleton object
    return instance;
}


void NetworkEngine::registerForStateUpdates(NetworkEngineListener* listener)
{
    mStateAccessor.registerForStateUpdates(listener);
}

void NetworkEngine::connect(void)
{
    mLoadBalancingClient.connect();
    mStateAccessor.setState(STATE_CONNECTING);
}

void NetworkEngine::disconnect(void)
{
    mLoadBalancingClient.disconnect();
}

void NetworkEngine::opCreateRoom(int givenRoomID)
{
    ExitGames::Common::JString roomName = static_cast<ExitGames::Common::JString>(givenRoomID);
    mLoadBalancingClient.opCreateRoom(roomName, true, true, 4, ExitGames::Common::Hashtable(), ExitGames::Common::JVector<ExitGames::Common::JString>(), ExitGames::Common::JString(), 1, INT_MAX/2, 10000);
    mStateAccessor.setState(STATE_JOINING);
    
    mLoadBalancingClient.service();
}

void NetworkEngine::opJoinRoom(int givenRoomID)
{
    ExitGames::Common::JString roomName = static_cast<ExitGames::Common::JString>(givenRoomID);
    mLoadBalancingClient.opJoinRandomRoom(ExitGames::Common::Hashtable(), 0, 0, roomName, 1, ExitGames::Common::JString());
    
    mLoadBalancingClient.service();
}

void NetworkEngine::opJoinRandomRoom(void)
{
    mLoadBalancingClient.opJoinRandomRoom();
}

void NetworkEngine::run(void)
{
    State state = mStateAccessor.getState();
    if(mLastInput == INPUT_EXIT && state != STATE_DISCONNECTING && state != STATE_DISCONNECTED)
    {
        disconnect();
        mStateAccessor.setState(STATE_DISCONNECTING);
    }
    else
    {
        switch(state)
        {
            case STATE_INITIALIZED:
                connect();
                mStateAccessor.setState(STATE_CONNECTING);
                break;
            case STATE_CONNECTING:
                break; // wait for callback
            case STATE_CONNECTED:
                switch(mLastInput)
            {
                case INPUT_1: // create Game
                {
                    if (roomID != 0)
                    {
                        opCreateRoom(roomID);
                    }
                }
                    break;
                case INPUT_2: // join Game
                {
                    if (roomID != 0)
                    {
                        opJoinRoom(roomID);
                    }
                    mStateAccessor.setState(STATE_JOINING);
                }
                    break;
                default: // no or illegal input -> stay waiting for legal input
                    break;
            }
                break;
            case STATE_JOINING:
                break; // wait for callback
            case STATE_JOINED:
                switch(mLastInput)
            {
                case INPUT_1: // leave Game
                    mLoadBalancingClient.opLeaveRoom();
                    mStateAccessor.setState(STATE_LEAVING);
                    break;
                case INPUT_2: // leave Game
                    mLoadBalancingClient.opLeaveRoom(true);
                    mStateAccessor.setState(STATE_LEAVING);
                    break;
                default: // no or illegal input -> stay waiting for legal input
                    break;
            }
                break;
            case STATE_LEAVING:
                break; // wait for callback
            case STATE_LEFT:
                break;
            case STATE_DISCONNECTING:
                break; // wait for callback
            case STATE_DISCONNECTED:
                connect();
                mStateAccessor.setState(STATE_CONNECTING);
                break;
            default:
                break;
        }
    }
    mLastInput = INPUT_NON;
    mLoadBalancingClient.service();
}

void NetworkEngine::sendEvent(nByte code, ExitGames::Common::Hashtable* eventContent)
{
    mLoadBalancingClient.opRaiseEvent(true, eventContent, 1, code);
}

// protocol implementations

void NetworkEngine::debugReturn(ExitGames::Common::DebugLevel::DebugLevel /*debugLevel*/, const ExitGames::Common::JString& string)
{
}

void NetworkEngine::connectionErrorReturn(int errorCode)
{
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
    mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkEngine::clientErrorReturn(int errorCode)
{
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
}

void NetworkEngine::warningReturn(int warningCode)
{
    EGLOG(ExitGames::Common::DebugLevel::WARNINGS, L"code: %d", warningCode);
}

void NetworkEngine::serverErrorReturn(int errorCode)
{
    EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
}

void NetworkEngine::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& /*playernrs*/, const ExitGames::LoadBalancing::Player& player)
{
    CCLOG("%ls joined the game", player.getName().cstr());
    ExitGames::Common::Hashtable* eventContent = new ExitGames::Common::Hashtable();
    eventContent->put<int, float>(1, player.getNumber());
    sendEvent(7, eventContent);
}

void NetworkEngine::leaveRoomEventAction(int playerNr, bool isInactive)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkEngine::disconnectEventAction(int playerNr)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkEngine::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    // you do not receive your own events, unless you specify yourself as one of the receivers explicitly, so you must start 2 clients, to receive the events, which you have sent, as sendEvent() uses the default receivers of opRaiseEvent() (all players in same room like the sender, except the sender itself)
    
    ExitGames::Common::Hashtable* event;
    
    switch (eventCode)
    {
            //update position
        case 1:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float x1 = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float y1 = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            float angle1 = ExitGames::Common::ValueObject<float>(event->getValue(3)).getDataCopy();
            float x2 = ExitGames::Common::ValueObject<float>(event->getValue(4)).getDataCopy();
            float y2 = ExitGames::Common::ValueObject<float>(event->getValue(5)).getDataCopy();
            float angle2 = ExitGames::Common::ValueObject<float>(event->getValue(6)).getDataCopy();
            float x3 = ExitGames::Common::ValueObject<float>(event->getValue(7)).getDataCopy();
            float y3 = ExitGames::Common::ValueObject<float>(event->getValue(8)).getDataCopy();
            int code = 1;
            
            std::vector<float> position;
            position.push_back(playerNr);
            position.push_back(x1);
            position.push_back(y1);
            position.push_back(angle1);
            position.push_back(x2);
            position.push_back(y2);
            position.push_back(angle2);
            position.push_back(x3);
            position.push_back(y3);
            position.push_back(code);
            eventQueue.push(position);
            
            break;
        }
            //shoot
        case 2:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float x = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float y = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            float angle = ExitGames::Common::ValueObject<float>(event->getValue(3)).getDataCopy();
            int code = 2;
            
            std::vector<float> bulletPosition;
            bulletPosition.push_back(playerNr);
            bulletPosition.push_back(x);
            bulletPosition.push_back(y);
            bulletPosition.push_back(angle);
            bulletPosition.push_back(code);
            eventQueue.push(bulletPosition);
            
            break;
        }
            //someone got hit
        case 3:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float victim = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            int code = 3;
            
            std::vector<float> victimN;
            victimN.push_back(playerNr);
            victimN.push_back(victim);
            victimN.push_back(code);
            eventQueue.push(victimN);
            
            break;
            
        }
            //turn event
        case 4:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            int direction = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            int code = 4;
            
            std::vector<float> turn;
            turn.push_back(playerNr);
            turn.push_back(direction);
            turn.push_back(code);
            eventQueue.push(turn);
            
            break;
        }
            //shield
        case 5:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float ship = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            int code = 5;
            
            std::vector<float> shield;
            shield.push_back(playerNr);
            shield.push_back(ship);
            shield.push_back(code);
            eventQueue.push(shield);
            
            break;
        }
            //GameOver
        case 6:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float score1 = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float score2 = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            int code = 6;
            
            std::vector<float> gameOver;
            gameOver.push_back(playerNr);
            gameOver.push_back(score1);
            gameOver.push_back(score2);
            gameOver.push_back(code);
            eventQueue.push(gameOver);
            
            break;
        }
            //Players joined
        case 7:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float players = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            int code = 7;
            
            std::vector<float> room;
            room.push_back(playerNr);
            room.push_back(players);
            room.push_back(code);
            eventQueue.push(room);
            
            break;
        }
            //All Joined
        case 8:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            int code = 8;
            
            std::vector<float> room;
            room.push_back(playerNr);
            room.push_back(code);
            eventQueue.push(room);
            
            break;
        }
            //HPup
        case 9:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float ship = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            int code = 9;
            
            std::vector<float> HPup;
            HPup.push_back(playerNr);
            HPup.push_back(ship);
            HPup.push_back(code);
            eventQueue.push(HPup);
            
            break;
        }
            //Double Damage
        case 10:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float ship = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            int code = 10;
            
            std::vector<float> doubleD;
            doubleD.push_back(playerNr);
            doubleD.push_back(ship);
            doubleD.push_back(code);
            eventQueue.push(doubleD);
            
            break;
        }
            //Invert Role
        case 11:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float ship = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            int code = 11;
            
            std::vector<float> invertR;
            invertR.push_back(playerNr);
            invertR.push_back(ship);
            invertR.push_back(code);
            eventQueue.push(invertR);
            
            break;
        }
            //Spawn Rune
        case 12:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float rune1 = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float rune2 = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            int code = 12;
            
            std::vector<float> rune;
            rune.push_back(playerNr);
            rune.push_back(rune1);
            rune.push_back(rune2);
            rune.push_back(code);
            eventQueue.push(rune);
            
            break;
        }
        //toggle invert role
        case 13:
        {
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            int code = 13;
            
            std::vector<float> invert;
            invert.push_back(playerNr);
            invert.push_back(code);
            eventQueue.push(invert);
            
            break;
        }
        default:
            break;
            
    }
    EGLOG(ExitGames::Common::DebugLevel::ALL, L"");
}

void NetworkEngine::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    if(errorCode)
    {
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
        mStateAccessor.setState(STATE_DISCONNECTING);
        return;
    }
    mStateAccessor.setState(STATE_CONNECTED);
}

void NetworkEngine::disconnectReturn(void)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkEngine::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    if(errorCode)
    {
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
        mStateAccessor.setState(STATE_CONNECTED);
        return;
    }
    mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
    mLastActorNr = localPlayerNr;
    playerNr = localPlayerNr;
    
    CCLOG("localPlayerNr: %d", localPlayerNr);
    mStateAccessor.setState(STATE_JOINED);
}

void NetworkEngine::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    if(errorCode)
    {
        mLastJoinedRoom = "";
        mLastActorNr = 0;
        playerNr = 0;
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
        mStateAccessor.setState(STATE_CONNECTED);
        return;
    }
    
    playerNr = localPlayerNr;
    
    CCLOG("localPlayerNr: %d", localPlayerNr);
    mStateAccessor.setState(STATE_JOINED);
}

void NetworkEngine::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    if(errorCode)
    {
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
        mStateAccessor.setState(STATE_CONNECTED);
        if (errorCode == 32760)
        {
            mStateAccessor.setState(STATE_ROOMFULL);
            
        }
        return;
    }
    
    mLastJoinedRoom = mLoadBalancingClient.getCurrentlyJoinedRoom().getName();
    mLastActorNr = localPlayerNr;
    playerNr = localPlayerNr;
    
    CCLOG("localPlayerNr: %d", localPlayerNr);
    mStateAccessor.setState(STATE_JOINED);
}

void NetworkEngine::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    if(errorCode)
    {
        EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
        mStateAccessor.setState(STATE_DISCONNECTING);
        return;
    }
    setRoomID(0);
    mStateAccessor.setState(STATE_LEFT);
}

void NetworkEngine::joinLobbyReturn(void)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkEngine::leaveLobbyReturn(void)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkEngine::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStats>& lobbyStats)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"onLobbyStatsUpdate: %ls", lobbyStats.toString().cstr());
}

void NetworkEngine::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStats>& lobbyStats)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"onLobbyStatsUpdate: %ls", lobbyStats.toString().cstr());
}

void NetworkEngine::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"onAvailableRegions: %ls", availableRegions.toString().cstr(), availableRegionServers.toString().cstr());
    
    // select asia region from list
    mLoadBalancingClient.selectRegion(availableRegions[2]);
}

bool NetworkEngine::isRoomExists(void)
{
    if (mLoadBalancingClient.getRoomList().getIsEmpty())
    {
        return false;
    }
    return true;
}

int NetworkEngine::getRoundTripTime(void)
{
    return mLoadBalancingClient.getRoundTripTime();
}

void NetworkEngine::setRoomID(int givenRoomID)
{
    roomID = givenRoomID;
}
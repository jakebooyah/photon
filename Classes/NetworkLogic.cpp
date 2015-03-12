#include "NetworkLogic.h"
#include "cocos2d.h"

static const ExitGames::Common::JString appId = L"78de70d1-ee79-46df-bc56-a296ef1a4535"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";

static const bool autoLobbbyStats = true;
static const bool useDefaultRegion = false;

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

ExitGames::Common::JString& NetworkLogicListener::toString(ExitGames::Common::JString& retStr, bool /*withTypes*/) const
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

void StateAccessor::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateUpdateListeners.addElement(listener);
}

Input NetworkLogic::getLastInput(void) const
{
	return mLastInput;
}

void NetworkLogic::setLastInput(Input newInput)
{
	mLastInput = newInput;
}

State NetworkLogic::getState(void) const
{
	return mStateAccessor.getState();
}

// functions
NetworkLogic::NetworkLogic(const ExitGames::LoadBalancing::AuthenticationValues& authenticationValues)
#ifdef _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
	: mLoadBalancingClient(*this, appId, appVersion, PLAYER_NAME+GETTIMEMS(), false, authenticationValues, autoLobbbyStats, useDefaultRegion)
	, mLastInput(INPUT_NON)
	, mLastActorNr(0)
    , playerNr(0)
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

void NetworkLogic::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateAccessor.registerForStateUpdates(listener);
}

void NetworkLogic::connect(void)
{
	mLoadBalancingClient.connect();
    mStateAccessor.setState(STATE_CONNECTING);
}

void NetworkLogic::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void NetworkLogic::opCreateRoom(void)
{
	// if last digits are always nearly the same, this is because of the timer calling this function being triggered every x ms with x being a factor of 10
	ExitGames::Common::JString tmp;
	mLoadBalancingClient.opCreateRoom(tmp=GETTIMEMS(), true, true, 4, ExitGames::Common::Hashtable(), ExitGames::Common::JVector<ExitGames::Common::JString>(), ExitGames::Common::JString(), 1, INT_MAX/2, 10000);
    mStateAccessor.setState(STATE_JOINING);
}

void NetworkLogic::opJoinRandomRoom(void)
{
	mLoadBalancingClient.opJoinRandomRoom();
}

void NetworkLogic::run(void)
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
                        opCreateRoom();
                        break;
                    case INPUT_2: // join Game
                        // remove false to enable rejoin
                        if(false && mLastJoinedRoom.length())
                        {
                            mLoadBalancingClient.opJoinRoom(mLastJoinedRoom, true, mLastActorNr);
                        }
                        else
                        {
                            opJoinRandomRoom();
                        }
                        mStateAccessor.setState(STATE_JOINING);
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
				mStateAccessor.setState(STATE_CONNECTED);
				break;
			case STATE_DISCONNECTING:
				break; // wait for callback
			default:
				break;        
		}
	}
	mLastInput = INPUT_NON;
	mLoadBalancingClient.service();
}

void NetworkLogic::sendEvent(nByte code, ExitGames::Common::Hashtable* eventContent)
{
    mLoadBalancingClient.opRaiseEvent(true, eventContent, 1, code);
}

// protocol implementations

void NetworkLogic::debugReturn(ExitGames::Common::DebugLevel::DebugLevel /*debugLevel*/, const ExitGames::Common::JString& string)
{
}

void NetworkLogic::connectionErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
    mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::clientErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
}

void NetworkLogic::warningReturn(int warningCode)
{
	EGLOG(ExitGames::Common::DebugLevel::WARNINGS, L"code: %d", warningCode);
}

void NetworkLogic::serverErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
}

void NetworkLogic::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& /*playernrs*/, const ExitGames::LoadBalancing::Player& player)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
}

void NetworkLogic::leaveRoomEventAction(int playerNr, bool isInactive)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkLogic::disconnectEventAction(int playerNr)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkLogic::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	// you do not receive your own events, unless you specify yourself as one of the receivers explicitly, so you must start 2 clients, to receive the events, which you have sent, as sendEvent() uses the default receivers of opRaiseEvent() (all players in same room like the sender, except the sender itself)
    
    ExitGames::Common::Hashtable* event;

    switch (eventCode)
    {
        case 1:
            event = ExitGames::Common::ValueObject< ExitGames::Common::Hashtable* >(eventContent).getDataCopy();
            float x1 = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float y1 = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            float angle1 = ExitGames::Common::ValueObject<float>(event->getValue(3)).getDataCopy();
            float x2 = ExitGames::Common::ValueObject<float>(event->getValue(4)).getDataCopy();
            float y2 = ExitGames::Common::ValueObject<float>(event->getValue(5)).getDataCopy();
            float angle2 = ExitGames::Common::ValueObject<float>(event->getValue(6)).getDataCopy();
            
            std::vector<float> crap;
            crap.push_back(playerNr);
            crap.push_back(x1);
            crap.push_back(y1);
            crap.push_back(angle1);
            crap.push_back(x2);
            crap.push_back(y2);
            crap.push_back(angle2);
            eventQueue.push(crap);
            
            break;
    }
    
	EGLOG(ExitGames::Common::DebugLevel::ALL, L"");
}

void NetworkLogic::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
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

void NetworkLogic::disconnectReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
    mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString)
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

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
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

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
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

	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);	
	mStateAccessor.setState(STATE_JOINED);
}

void NetworkLogic::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
        mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mStateAccessor.setState(STATE_LEFT);
}

void NetworkLogic::joinLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkLogic::leaveLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkLogic::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStats>& lobbyStats)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"onLobbyStatsUpdate: %ls", lobbyStats.toString().cstr());
}

void NetworkLogic::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStats>& lobbyStats)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"onLobbyStatsUpdate: %ls", lobbyStats.toString().cstr());
}

void NetworkLogic::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"onAvailableRegions: %ls", availableRegions.toString().cstr(), availableRegionServers.toString().cstr());

    // select asia region from list
    mLoadBalancingClient.selectRegion(availableRegions[2]);
}

bool NetworkLogic::isRoomExists(void)
{
    if (mLoadBalancingClient.getRoomList().getIsEmpty())
    {
        return false;
    }
    
    return true;
}

int NetworkLogic::getRoundTripTime(void)
{
    return mLoadBalancingClient.getRoundTripTime();
}
//
//  NetworkEngine.h
//  photon
//
//  Created by Jake on 4/3/15.
//
//

#ifndef __photon__NetworkEngine__
#define __photon__NetworkEngine__

#include "LoadBalancing-cpp/inc/Client.h"
#include <vector>
#include <queue>

enum State
{
    STATE_INITIALIZED = 0,
    STATE_CONNECTING,
    STATE_CONNECTED,
    STATE_JOINING,
    STATE_JOINED,
    STATE_LEAVING,
    STATE_LEFT,
    STATE_DISCONNECTING,
    STATE_DISCONNECTED,
    STATE_ROOMFULL
};

enum Input
{
    INPUT_NON = 0,
    INPUT_1,
    INPUT_2,
    INPUT_EXIT
};

class NetworkEngineListener : public ExitGames::Common::ToString
{
public:
    using ExitGames::Common::ToString::toString;
    virtual void stateUpdate(State newState) = 0;
    virtual ExitGames::Common::JString& toString(ExitGames::Common::JString& retStr, bool withTypes=false) const;
};

class StateAccessor
{
public:
    State getState(void) const;
    void setState(State newState);
    void registerForStateUpdates(NetworkEngineListener* listener);
    
private:
    State mState;
    ExitGames::Common::JVector<NetworkEngineListener*> mStateUpdateListeners;
};



class NetworkEngine : private ExitGames::LoadBalancing::Listener
{
public:
    static NetworkEngine* getInstance();
    NetworkEngine(const ExitGames::LoadBalancing::AuthenticationValues& authenticationValues=ExitGames::LoadBalancing::AuthenticationValues());
    void registerForStateUpdates(NetworkEngineListener* listener);
    void run(void);
    void connect(void);
    void opCreateRoom(int);
    void opJoinRoom(int);
    void opJoinRandomRoom(void);
    void disconnect(void);
    void sendEvent(nByte code, ExitGames::Common::Hashtable* eventContent);
    bool isRoomExists(void);
    int getRoundTripTime(void);
    
    int playerNr;
    std::queue< std::vector<float> >eventQueue;
    
    Input getLastInput(void) const;
    void setLastInput(Input newInput);
    
    void setRoomID(int);
    
    State getState(void) const;
private:
    static NetworkEngine *instance;
    
    // receive and print out debug out here
    virtual void debugReturn(ExitGames::Common::DebugLevel::DebugLevel debugLevel, const ExitGames::Common::JString& string);
    
    // implement your error-handling here
    virtual void connectionErrorReturn(int errorCode);
    virtual void clientErrorReturn(int errorCode);
    virtual void warningReturn(int warningCode);
    virtual void serverErrorReturn(int errorCode);
    
    // events, triggered by certain operations of all players in the same room
    virtual void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player);
    virtual void leaveRoomEventAction(int playerNr, bool isInactive);
    virtual void disconnectEventAction(int playerNr);
    virtual void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent);
    
    virtual void onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStats>& lobbyStats);
    virtual void onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStats>& lobbyStats);
    virtual void onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers);
    
    // callbacks for operations on PhotonLoadBalancing server
    virtual void connectReturn(int errorCode, const ExitGames::Common::JString& errorString);
    virtual void disconnectReturn(void);
    virtual void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& gameProperties, const ExitGames::Common::Hashtable& playerProperties, int errorCode, const ExitGames::Common::JString& errorString);
    virtual void leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString);
    virtual void joinLobbyReturn(void);
    virtual void leaveLobbyReturn(void);
    
    ExitGames::LoadBalancing::Client mLoadBalancingClient;
    ExitGames::Common::JString mLastJoinedRoom;
    int mLastActorNr;
    int roomID;
    ExitGames::Common::Logger mLogger;
    StateAccessor mStateAccessor;
    Input mLastInput;
};

#endif /* defined(__photon__NetworkEngine__) */

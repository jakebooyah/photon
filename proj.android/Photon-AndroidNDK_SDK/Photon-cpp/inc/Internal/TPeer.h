/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2015 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/Common.h"
#include "Photon-cpp/inc/Internal/PeerBase.h"
#include "Photon-cpp/inc/OperationRequest.h"
#include "Photon-cpp/inc/Internal/Enums/PhotonCommandProperties.h"

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class TPeer: public PeerBase
			{
			public:
				TPeer(PhotonListener& listener, const Common::Logger& logger);
				~TPeer(void);
			private:
				typedef PeerBase super;

				class Operation
				{
				public:
					Operation();
					~Operation();

					Operation(const Operation& toCopy);
					Operation& operator=(const Operation& toCopy);

					void setData(const nByte* data, unsigned int sz);
					Common::JString toString(bool withTypes);

					nByte* mpData;
					unsigned int mSize;
				};

				void cleanupNonHierarchical(void);
				void cleanup(void);
				void reset(void);
				bool connect(const EG_CHAR* ipAddr, const nByte appID[32]);
				bool startConnection(const char* ipAddr);
				void disconnect(void);
				void stopConnection(void);
				void serviceBasic(void);
				bool sendOutgoingCommands(void);
				bool sendAcksOnly(void);
				bool dispatchIncomingCommands(void);
				void fetchServerTimestamp(void);

				int getIncomingReliableCommandsCount(void) const;
				int getQueuedIncomingCommands(void) const;
				int getQueuedOutgoingCommands(void) const;

				void onConnect(int nError);
				void onReceiveData(nByte* pBuf, int iCount, int nError);

				void send(nByte cType, nByte* payload, unsigned int payloadSize, nByte channelId);
				bool serializeOperation(const OperationRequest* pOperationRequest, nByte** buffer,  int* buffSize, bool encrypt, nByte msgType);
				void clearAllQueues(void);
				void initCallback(void);

				void readPingFromBuffer(nByte* pBuffer, int* nRead);
				void sendPing(void);
				void sendInit(void);

				int mTimeLastPing;
				Common::JVector<Operation> mOutgoingOpList;
				Common::JVector<Operation> mIncomingOpList;
				static const nByte mTcpHeader[PhotonCommandProperties::TCP_HEADER_LENGTH+PhotonCommandProperties::MSG_HEADER_BYTES];
			};
		}
	}
}
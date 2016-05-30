#pragma region Copyright (c) 2014-2016 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#pragma once

#include <list>
#include <memory>
#include <vector>
#include "NetworkTypes.h"
#include "NetworkKey.h"
#include "NetworkPacket.h"
#include "../common.h"

class NetworkPlayer;

class NetworkConnection
{
public:
    SOCKET              Socket          = INVALID_SOCKET;
    NetworkPacket       InboundPacket;
    NETWORK_AUTH        AuthStatus      = NETWORK_AUTH_NONE;
    NetworkPlayer *     Player          = nullptr;
    uint32              PingTime        = 0;
    NetworkKey          Key;
    std::vector<uint8>  Challenge;

    NetworkConnection();
    ~NetworkConnection();

    int  ReadPacket();
    void QueuePacket(std::unique_ptr<NetworkPacket> packet, bool front = false);
    void SendQueuedPackets();
    bool SetTCPNoDelay(bool on);
    bool SetNonBlocking(bool on);
    void ResetLastPacketTime();
    bool ReceivedPacketRecently();

    const utf8 * GetLastDisconnectReason() const;
    void SetLastDisconnectReason(const utf8 * src);
    void SetLastDisconnectReason(const rct_string_id string_id, void * args = nullptr);

    static bool SetNonBlocking(SOCKET socket, bool on);

private:
    std::list<std::unique_ptr<NetworkPacket>>   _outboundPackets;
    uint32                                      _lastPacketTime;
    utf8 *                                      _lastDisconnectReason   = nullptr;

    bool SendPacket(NetworkPacket &packet);
};

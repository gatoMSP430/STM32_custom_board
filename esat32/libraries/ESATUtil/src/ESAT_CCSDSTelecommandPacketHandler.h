/*
 * Copyright (C) 2018 Theia Space, Universidad Politécnica de Madrid
 *
 * This file is part of Theia Space's ESAT Util library.
 *
 * Theia Space's ESAT Util library is free software: you can
 * redistribute it and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Theia Space's ESAT Util library is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Theia Space's ESAT Util library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef ESAT_CCSDSTelecommandPacketHandler_h
#define ESAT_CCSDSTelecommandPacketHandler_h

#include <Arduino.h>
#include "ESAT_CCSDSPacket.h"
#include "ESAT_SemanticVersionNumber.h"

// Packet handler interface.
// Use together with ESAT_CCSDSTelecommandPacketDispatcher to handle
// telecommand packets.
class ESAT_CCSDSTelecommandPacketHandler
{
  public:
    // Next packet handler object in the list of packet handler objects.
    // ESAT_CCSDSTelecommandPacketDispatcher uses this to keep a
    // linked list of registered packet handlers: it can traverse the
    // list by going from one packet handler object to the next packet
    // handler object until reaching the end of the list at nullptr.
    // Only ESAT_CCSDSTelecommandPacketDispatcher should care about
    // this.  Each packet handler object should be added just one time
    // to just one ESAT_CCSDSTelecommandPacketDispatcher object.
    ESAT_CCSDSTelecommandPacketHandler* nextTelecommandPacketHandler;

    // Trivial destructor.
    // We need to define it because the C++ programming language
    // works this way.
    virtual ~ESAT_CCSDSTelecommandPacketHandler() {};

    // Consume/process a packet.
    // The read/write pointer of the packet is at the start of the
    // user data field.
    // Return true on success; otherwise return false.
    virtual boolean handleUserData(ESAT_CCSDSPacket packet) = 0;

    // Return the packet identifier of this packet handler.
    // ESAT_CCSDSTelecommandPacketDispatcher objects pass telecommand
    // packets to a handler object only when the packet identifiers
    // match.
    virtual byte packetIdentifier() = 0;

    // Return the version number of this packet handler.
    // ESAT_CCSDSTelecommandPacketDispatcher objects pass telecommand
    // packets to a handler object only when the packet version number
    // is backward-compatible with the handler version number.
    virtual ESAT_SemanticVersionNumber versionNumber() = 0;
};

#endif /* ESAT_CCSDSTelecommandPacketHandler_h */

/*
 * Copyright (C) 2018, 2019 Theia Space, Universidad Politécnica de Madrid
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

#ifndef ESAT_CCSDSTelecommandPacketDispatcher_h
#define ESAT_CCSDSTelecommandPacketDispatcher_h

#include <Arduino.h>
#include "ESAT_CCSDSPacket.h"
#include "ESAT_CCSDSTelecommandPacketHandler.h"
#include "ESAT_SemanticVersionNumber.h"

// Telecommand packet dispatcher for ESAT's CCSDS space packets.
class ESAT_CCSDSTelecommandPacketDispatcher
{
  public:
    // Set up the telecommand packet dispatcher for the given application
    // process identifier.
    // The telecommand dispatcher will only handle a packet if the
    // following conditions are met:
    // - The packet is a telecommand packet.
    // - The packet has a secondary header.
    // - The packet's application process identifier is the same
    //   as the telecommand dispatcher's application process identifier.
    ESAT_CCSDSTelecommandPacketDispatcher(word applicationProcessIdentifier = 0);

    // Add a new entry to the list of packet handlers.
    void add(ESAT_CCSDSTelecommandPacketHandler& handler);

    // Return true if the packet is compatible; otherwise return
    // false.
    // A packet is compatible with this telecommand dispatcher when
    // all the following conditions are met:
    // - The packet is a telecommand packet.
    // - The packet has a secondary header.
    // - The packet's application process identifier is the same
    //   as the telecommand dispatcher's application process identifier.
    boolean compatiblePacket(ESAT_CCSDSPacket packet) const;

    // Dispatch a telecommand packet.
    // This will work through the list of packet handlers until one
    // matches the packet identifier and is compatible with the
    // version number.
    // The packet will be passed to the compatible handler with the
    // read/write pointer at the start of the user data field.
    // The telecommand dispatcher will fail to dispatch a packet if any
    // of the following conditions isn't met:
    // - The packet is a telecommand packet.
    // - The packet has a secondary header.
    // - The packet's application process identifier is the same
    //   as the telecommand dispatcher's application process identifier.
    // - The packet is compatible with a handler.
    // - The handler handles the packet successfully.
    // Return true on success; otherwise return false.
    boolean dispatch(ESAT_CCSDSPacket packet);

  private:
    // Application process identifier.
    // Each logical subsystem should have its own unique application
    // process identifier (e.g., the attitude determination and
    // control subsystem has its own application process identifier).
    word applicationProcessIdentifier;

    // Head of the list of packet handler objects.
    ESAT_CCSDSTelecommandPacketHandler* head;

    // Return true if the handler is compatible with the packet with
    // given secondary header; otherwise return false.
    boolean handlerIsCompatibleWithPacket(ESAT_CCSDSTelecommandPacketHandler& handler,
                                          ESAT_CCSDSSecondaryHeader secondaryHeader);
};

#endif /* ESAT_CCSDSTelecommandPacketDispatcher_h */

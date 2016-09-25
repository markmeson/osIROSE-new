// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http ://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * \file findclass.h
 *
 * \author L3nn0x
 * \date april 2016
 *
 * This header contains two structs to properly deduce the subclass type of a packet from the ePacketType value
 */
#ifndef _FINDCLASS_H_
#define _FINDCLASS_H_

#include "epackettype.h"
#include "packetclasses.h"

namespace RoseCommon {

//RECEIVE PACKETS
template <ePacketType T>
struct find_recv_class {};

template <>
struct find_recv_class<ePacketType::PAKCS_SCREEN_SHOT_TIME_REQ> { CliScreenShotReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_JOIN_SERVER_REQ> { CliJoinServerReq type; };

//SEND PACKETS
template <ePacketType T>
struct find_send_class {};

template <>
struct find_send_class<ePacketType::PAKSS_ACCEPT_REPLY> { SrvAcceptClient type; };

template <>
struct find_send_class<ePacketType::PAKSC_SCREEN_SHOT_TIME_REPLY> { SrvScreenShotReply type; };

// CHAR/MAP SERVER
template <>
struct find_send_class<ePacketType::PAKSC_JOIN_SERVER_REPLY> { SrvJoinServerReply type; };

template <>
struct find_send_class<ePacketType::PAKCC_SWITCH_SERVER> { SrvSwitchServerReply type; };

//-----------------------------------------------
// ISC Packets
//-----------------------------------------------
template <>
struct find_recv_class<ePacketType::ISC_SERVER_REGISTER> { IscServerRegister type; };

template <>
struct find_send_class<ePacketType::ISC_SERVER_REGISTER> { IscServerRegister type; };

template <>
struct find_recv_class<ePacketType::ISC_SHUTDOWN> { IscServerShutdown type; };

template <>
struct find_send_class<ePacketType::ISC_SHUTDOWN> { IscServerShutdown type; };
}

#endif /* !_FINDCLASS_H_ */

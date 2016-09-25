
#ifndef _FINDCLASSMAP_H_
# define _FINDCLASSMAP_H_

#include "findclass.h"

namespace RoseCommon {
template <>
struct find_recv_class<ePacketType::PAKCS_LOGOUT_REQ> { CliLogoutReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_STOP_MOVING> { CliStopMoving type; };

template <>
struct find_recv_class<ePacketType::PAKCS_MOUSE_CMD> { CliMouseCmd type; };

template <>
struct find_recv_class<ePacketType::PAKCS_CHANGE_MAP_REQ> { CliChangeMapReq type; };

template <>
struct find_recv_class<ePacketType::PAKWC_GLOBAL_VARS> { SrvServerData type; };

template <>
struct find_recv_class<ePacketType::PAKCS_REVIVE_REQ> { CliReviveReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_NORMAL_CHAT> { CliChat type; };

//------------------------------------------------------------------------------

template <>
struct find_send_class<ePacketType::PAKWC_SELECT_CHAR_REPLY> { SrvSelectCharReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_MOUSE_CMD> { SrvMouseCmd type; };

template <>
struct find_send_class<ePacketType::PAKWC_INVENTORY_DATA> { SrvInventoryData type; };

template <>
struct find_send_class<ePacketType::PAKWC_QUEST_DATA> { SrvQuestData type; };

template <>
struct find_send_class<ePacketType::PAKWC_BILLING_MESSAGE> { SrvBillingMsg type; };

template <>
struct find_send_class<ePacketType::PAKWC_LOGOUT_REPLY> { SrvLogoutReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_INIT_DATA> { SrvInitDataReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_CHANGE_MAP_REPLY> { SrvChangeMapReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_REVIVE_REPLY> { SrvReviveReply type; };

template <>
struct find_send_class<ePacketType::PAKWC_NORMAL_CHAT> { SrvChat type; };
}

#endif /* !_FINDCLASSMAP_H_ */

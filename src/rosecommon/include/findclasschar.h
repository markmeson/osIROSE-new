
#ifndef _FINDCLASSCHAR_H_
# define _FINDCLASSCHAR_H_

#include "findclass.h"

namespace RoseCommon {

template <>
struct find_recv_class<ePacketType::PAKCS_CREATE_CHAR_REQ> { CliCreateCharReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_DELETE_CHAR_REQ> { CliDeleteCharReq type; };

template <>
struct find_recv_class<ePacketType::PAKCS_SELECT_CHAR_REQ> { CliSelectCharReq type; };

//----------------------------------------------------------------------------------

template <>
struct find_send_class<ePacketType::PAKCC_CHAR_LIST_REPLY> { SrvCharacterListReply type; };

template <>
struct find_send_class<ePacketType::PAKCC_CREATE_CHAR_REPLY> { SrvCreateCharReply type; };

template <>
struct find_send_class<ePacketType::PAKCC_DELETE_CHAR_REPLY> { SrvDeleteCharReply type; };
}

#endif /* !_FINDCLASSCHAR_H_ */

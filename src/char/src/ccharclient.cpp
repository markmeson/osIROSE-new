#include "ccharserver.h"
#include "ccharisc.h"
#include "ccharclient.h"
#include "epackettype.h"
#include "rosepackets.h"

using namespace RoseCommon;

CCharClient::CCharClient() : CRoseClient(), access_rights_(0), login_state_(eSTATE::DEFAULT) {
}

CCharClient::CCharClient(tcp::socket _sock)
    : CRoseClient(std::move(_sock)), access_rights_(0), login_state_(eSTATE::DEFAULT) {
}

bool CCharClient::HandlePacket(uint8_t* _buffer) {
  switch (CRosePacket::type(_buffer)) {
    case ePacketType::PAKCS_JOIN_SERVER_REQ:
      return JoinServerReply(getPacket<ePacketType::PAKCS_JOIN_SERVER_REQ>(_buffer));  // Allow client to connect
    case ePacketType::PAKCS_CHAR_LIST_REQ:
      return SendCharListReply();
    case ePacketType::PAKCS_CREATE_CHAR_REQ:
      return SendCharCreateReply(getPacket<ePacketType::PAKCS_CREATE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_DELETE_CHAR_REQ:
      return SendCharDeleteReply(getPacket<ePacketType::PAKCS_DELETE_CHAR_REQ>(_buffer));
    case ePacketType::PAKCS_SELECT_CHAR_REQ:
      return SendCharSelectReply(getPacket<ePacketType::PAKCS_SELECT_CHAR_REQ>(_buffer));
    default:
      return CRoseClient::HandlePacket(_buffer);
  }
  return true;
}

bool CCharClient::OnReceived() { return CRoseClient::OnReceived(); }

bool CCharClient::JoinServerReply(std::unique_ptr<RoseCommon::CliJoinServerReq> P) {
  (void)P;
  logger_->trace("JoinServerReply\n");

  //uint32_t channelID = P->id();

  //ask the login server if this client actually logged in successfully and selected that channel

  auto packet = makePacket<ePacketType::PAKSC_JOIN_SERVER_REPLY>(SrvJoinServerReply::OK, std::time(nullptr));
  Send(*packet);

  return true;
}

bool CCharClient::SendCharListReply() {
  logger_->trace("CharListReply\n");

  //mysql query to get the characters created.

  auto packet = makePacket<ePacketType::PAKCC_CHAR_LIST_REPLY>();
  packet->addCharacter("Raven", 1, 1, 0, 0);
  Send(*packet);

  return true;
}

bool CCharClient::SendCharCreateReply(std::unique_ptr<RoseCommon::CliCreateCharReq> P) {
  (void)P;
  logger_->trace("CharCreateReply\n");

  return true;
}

bool CCharClient::SendCharDeleteReply(std::unique_ptr<RoseCommon::CliDeleteCharReq> P) {
  logger_->trace("CharDeleteReply\n");

  if( P->char_id() > 6 )
    return false;

  uint32_t time = 0;
  if(P->isDelete()) {
    // we need to delete the char
  }

  auto packet = makePacket<ePacketType::PAKCC_DELETE_CHAR_REPLY>( P->name(), time );
  Send( *packet );
  return true;
}

bool CCharClient::SendCharSelectReply(std::unique_ptr<RoseCommon::CliSelectCharReq> P) {
  (void)P;
  logger_->trace("CharSelectReply\n");

  return true;
}

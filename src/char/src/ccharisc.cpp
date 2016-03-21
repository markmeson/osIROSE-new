#include "ccharisc.h"
#include "crosepacket.h"
#include "ccharserver.h"
#include "config.h"

using namespace RoseCommon;

CCharISC::CCharISC() : CRoseISC(), login_connection_(false) {

}

CCharISC::CCharISC(tcp::socket _sock)
    : CRoseISC(std::move(_sock)), login_connection_(false) {

}

bool CCharISC::HandlePacket(uint8_t* _buffer) {
  CRosePacket* pak = (CRosePacket*)_buffer;
  switch (pak->Header.Command) {
    case ePacketType::ISC_ALIVE:
      return true;
    case ePacketType::ISC_SERVER_AUTH:
      return true;
    case ePacketType::ISC_SERVER_REGISTER:
      return ServerRegister(pak);
    case ePacketType::ISC_TRANSFER:
      return true;
    case ePacketType::ISC_SHUTDOWN:
      return true;
    default: {
      CRoseISC::HandlePacket(_buffer);
      return false;
    }
  }
  return true;
}

bool CCharISC::ServerRegister(CRosePacket* P) {
  CRosePacket* pak = (CRosePacket*)P;
  uint16_t _insize = pak->Header.Size - 6;

  ServerReg pMapServer;
  if (pMapServer.ParseFromArray(pak->Data, _insize) == false) return false;

  int16_t _type = 0;
  _type = pMapServer.type();

  // 1 == char server
  // 2 == node server
  // 3 == map master server (This is the only type the login server will care
  // about)
  // 4 == map workers/threads

  ServerReg pServerReg;

  if (_type == 2) {
    // This is a node and we need to figure out something to do with this
  } else if (_type == 3) {
    pServerReg.set_name(pMapServer.name());
    pServerReg.set_addr(pMapServer.addr());
    pServerReg.set_port(pMapServer.port());
    pServerReg.set_type(pMapServer.type());
    pServerReg.set_accright(pMapServer.accright());
  }

  CRosePacket* pakToLS = new CRosePacket(ePacketType::ISC_SERVER_REGISTER);
  int _size = pServerReg.ByteSize();
  uint8_t* data = new uint8_t[_size];
  memset(data, 0, _size);
  if (pServerReg.SerializeToArray(data, _size) == false)
    logger_->error("Couldn't serialize the data");
  pakToLS->AddBytes(data, _size);

  // todo: get the ISC connection to the login server and send the packet to it
  std::lock_guard<std::mutex> lock(CCharServer::GetISCListMutex());
  for (auto& server : CCharServer::GetISCList()) {
    CCharISC* svr = (CCharISC*)server;
    if (svr->IsLogin()) {
      svr->Send((CRosePacket*)pakToLS);
      delete[] data;
      return true;
    }
  }
  delete[] data;
  return false;
}

void CCharISC::OnConnected() {
  CRosePacket* pak = new CRosePacket(ePacketType::ISC_SERVER_REGISTER);

  Core::Config& config = Core::Config::getInstance();

  ServerReg pServerReg;
  pServerReg.set_name(config.char_server().worldname());
  pServerReg.set_addr(config.serverdata().ip());
  pServerReg.set_port(config.char_server().clientport());
  pServerReg.set_type(ServerReg_ServerType_CHAR);
  pServerReg.set_accright(config.char_server().accesslevel());

  int _size = pServerReg.ByteSize();
  uint8_t* data = new uint8_t[_size];
  memset(data, 0, _size);
  if (pServerReg.SerializeToArray(data, _size) == false)
    logger_->error("Couldn't serialize the data");
  pak->AddBytes(data, _size);

  logger_->trace("Sent a packet on CRoseISC: Header[{0}, 0x{1:x}]",
                 pak->Header.Size, (uint16_t)pak->Header.Command);

  Send((CRosePacket*)pak);
  delete[] data;
}
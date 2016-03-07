#include "cloginserver.h"
#include "cloginclient.h"
#include "cloginisc.h"
#include "ePacketType.h"
#include "config.h"

CLoginServer::CLoginServer(bool _isc) : CRoseServer(_isc), client_count_(0) {
  if (true == _isc)
    log_.SetIdentity("CLoginISCServer");
  else
    log_.SetIdentity("CLoginServer");

  Core::Config& config = Core::Config::getInstance();
  const ::configFile::Database& dbb = config.database();

  std::string host = dbb.host();
  std::string database = dbb.database();
  std::string user = dbb.user();
  std::string pass = dbb.password();

  database_ = std::unique_ptr<Core::CMySQL_Database>( new Core::CMySQL_Database( host, database, user, pass ) );
}

CLoginServer::~CLoginServer() { Shutdown(); }

void CLoginServer::OnAccepted(tcp::socket _sock) {
  if (_sock.is_open()) {
    // Do Something?
    std::string _address = _sock.remote_endpoint().address().to_string();
    if (IsISCServer() == false) {
      std::lock_guard<std::mutex> lock(client_list_mutex_);
      CLoginClient* nClient = new CLoginClient(std::move(_sock));
      nClient->SetId(client_count_++);
      log_.icprintf("[%d] Client connected from: %s\n", nClient->GetId(),
                    _address.c_str());
      client_list_.push_front(nClient);
    } else {
      std::lock_guard<std::mutex> lock(isc_list_mutex_);
      CLoginISC* nClient = new CLoginISC(std::move(_sock));
      log_.icprintf("Server connected from: %s\n", _address.c_str());
      isc_list_.push_front(nClient);
    }
  }
}
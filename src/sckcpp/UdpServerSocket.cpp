//
// Created by brd6 on 02/10/17.
//

#include "sckcpp/UdpServerSocket.hpp"

namespace sckcpp
{
  namespace udp
  {
    ServerSocket::ServerSocket(boost::asio::io_service &ios) :
     ClientSocket(ios)
    {
    }

    void ServerSocket::bind(std::string const &host, unsigned short port)
    {
      boost::asio::ip::udp::endpoint endpoint(
       boost::asio::ip::address::from_string(host.c_str()), port);

      mSocket.open(endpoint.protocol());
      mSocket.bind(endpoint);
    }

    void ServerSocket::run()
    {
      mIos.run();
    }

    void ServerSocket::stop()
    {
      mIos.stop();
    }
  }
}

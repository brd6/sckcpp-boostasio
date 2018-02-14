//
// Created by brd6 on 28/09/17.
//

#include <utility>
#include "sckcpp/ServerSocket.hpp"
#include "sckcpp/TcpServerSocket.hpp"

namespace sckcpp
{
  namespace tcp
  {
    ServerSocket::ServerSocket(boost::asio::io_service &ios) :
     mIos(ios),
     mAcceptor(mIos)
    {
    }

    void ServerSocket::bind()
    {
      bind(0);
    }

    void ServerSocket::bind(unsigned short port)
    {
      bind("", port);
    }

    void ServerSocket::bind(std::string const &host, unsigned short port)
    {
      auto endpoint = createEndpointFromAddress(host, port);

      mAcceptor.open(endpoint.protocol());
      mAcceptor.bind(endpoint);
    }

    void ServerSocket::enableReUseAddress()
    {
      mAcceptor.set_option(
       boost::asio::ip::tcp::acceptor::reuse_address(true));
    }

    void ServerSocket::listen(int backlog)
    {
      mAcceptor.listen(backlog);
    }

    void ServerSocket::run()
    {
      mIos.run();
    }

    void ServerSocket::stop()
    {
      mIos.stop();
    }

    void ServerSocket::asyncAccept(OnClientAcceptedFunc onClientAccepted)
    {
      if (onClientAccepted != nullptr)
	this->mOnClientAccepted = onClientAccepted;
      startAsyncAccept();
    }

    ClientPtr ServerSocket::accept()
    {
      auto newClient = helper::createClient(mAcceptor.get_io_service());

      mAcceptor.accept(newClient->getSocket());

      return newClient;
    }

    void ServerSocket::startAsyncAccept()
    {
      auto newClient = helper::createClient(mAcceptor.get_io_service());

      mAcceptor.async_accept(newClient->getSocket(),
			     [this, newClient](
			      boost::system::error_code const &ec)
			     {

			       if (!ec && mOnClientAccepted != nullptr)
				 {
				   mOnClientAccepted(newClient);
				 }

			       startAsyncAccept();
			     });
    }

    boost::asio::ip::tcp::endpoint
    ServerSocket::createEndpointFromAddress(std::string const &host,
					    unsigned short port)
    {
      if (host.empty())
	return {boost::asio::ip::tcp::v4(), port};

      return {boost::asio::ip::address::from_string(host), port};
    }

    void ServerSocket::setOnClientAcceptedCallback(
     OnClientAcceptedFunc onClientAccepted)
    {
      mOnClientAccepted = std::move(onClientAccepted);
    }

    std::string ServerSocket::getAddress() const
    {
      return mAcceptor.local_endpoint().address().to_string();
    }

    unsigned short ServerSocket::getPort() const
    {
      return mAcceptor.local_endpoint().port();
    }

    std::string ServerSocket::getInfo()
    {
      return getAddress() + ":" + std::to_string(getPort());
    }

  }
}
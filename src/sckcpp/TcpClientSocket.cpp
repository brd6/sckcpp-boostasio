//
// Created by brd6 on 28/09/17.
//

#include <iostream>
#include <utility>
#include "sckcpp/TcpClientSocket.hpp"

namespace sckcpp
{
  namespace tcp
  {
    ClientSocket::ClientSocket(boost::asio::io_service &ios) :
     mIos(ios),
     mSocket(mIos),
     mIsConnected(false),
     mAddress(""),
     mPort(0)
    {
    }

    void ClientSocket::connect(const std::string &host, unsigned short port)
    {
      mSocket.connect({boost::asio::ip::address::from_string(host), port});
      mIsConnected = true;

      mAddress = mSocket.remote_endpoint().address().to_string();
      mPort = mSocket.remote_endpoint().port();
    }

    void ClientSocket::close()
    {
      if (!mIsConnected)
	return;

      try
	{
	  mSocket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	  mSocket.close();
	}
      catch (std::exception)
	{

	}
      mIsConnected = false;

    }

    std::size_t ClientSocket::write(void const *data, size_t size)
    {
      boost::system::error_code errorCode;
      std::size_t ret;

      ret = boost::asio::write(mSocket, boost::asio::buffer(data, size),
			       errorCode);

      return (!errorCode ? ret : 0);
    }

    boost::asio::ip::tcp::socket &ClientSocket::getSocket()
    {
      return mSocket;
    }

    std::size_t ClientSocket::read(void *data, size_t size)
    {
      boost::system::error_code errorCode;
      std::size_t ret;

      ret = boost::asio::read(mSocket, boost::asio::buffer(data, size),
			      errorCode);

      return (!errorCode ? ret : 0);
    }

    void ClientSocket::asyncWrite(void const *data, size_t size,
				  OnWriteFunc onWrite)
    {
      if (onWrite != nullptr)
	mOnWrite = onWrite;

      boost::asio::async_write(mSocket, boost::asio::buffer(data, size),
			       [=](const boost::system::error_code &ec,
				   size_t ret)
			       {

				 if (mOnWrite != nullptr)
				   {
				     mOnWrite(ret);
				   }

			       });
    }

    bool ClientSocket::isConnected() const
    {
      return mIsConnected;
    }

    void ClientSocket::asyncRead(void *data, size_t size, OnReadFunc onRead)
    {
      if (onRead != nullptr)
	mOnRead = onRead;

      boost::asio::async_read(mSocket, boost::asio::buffer(data, size),
			      [=](const boost::system::error_code &ec,
				  size_t ret)
			      {

				if (isDisconnectRequest(ec))
				  invokeCallback(mOnDisconnect);
				else
				  invokeCallback(mOnRead, ret);
			      });
    }

    void ClientSocket::setOnReadCallback(OnReadFunc onRead)
    {
      mOnRead = std::move(onRead);
    }

    void ClientSocket::setOnWriteCallback(OnWriteFunc onWrite)
    {
      mOnWrite = std::move(onWrite);
    }

    std::string const &ClientSocket::getAddress()
    {
      initializeAddressAndPort();

      return mAddress;
    }

    unsigned short ClientSocket::getPort()
    {
      initializeAddressAndPort();

      return mPort;
    }

    void
    ClientSocket::asyncConnect(std::string const &host, unsigned short port,
			       OnConnectFunc onConnect)
    {
      if (onConnect != nullptr)
	this->mOnConnect = onConnect;

      startAsyncConnect(host, port);
    }

    void ClientSocket::startAsyncConnect(std::string const &host,
					 unsigned short port)
    {
      mSocket.async_connect(
       {boost::asio::ip::address::from_string(host), port},
       [=](const boost::system::error_code &ec)
       {

	 if (!ec && mOnConnect != nullptr)
	   {
	     mIsConnected = true;

	     mOnConnect();
	   }
       });
    }

    void ClientSocket::setOnConnectCallback(OnConnectFunc onConnect)
    {
      mOnConnect = onConnect;
    }

    void ClientSocket::run()
    {
      mIos.run();
    }

    bool ClientSocket::isDisconnectRequest(const boost::system::error_code &ec)
    {
      return ((boost::asio::error::eof == ec) ||
	      (boost::asio::error::connection_reset == ec));
    }

    void ClientSocket::setOnDisconnectCallback(OnDisconnect onDisconnect)
    {
      mOnDisconnect = onDisconnect;
    }

    std::string ClientSocket::getInfo()
    {
      return {getAddress() + ":" + std::to_string(getPort())};
    }

    void ClientSocket::initializeAddressAndPort()
    {
      if ((!mAddress.empty() && mPort > 0) || !mIsConnected)
	return;

      mAddress = mSocket.remote_endpoint().address().to_string();
      mPort = mSocket.remote_endpoint().port();
    }

    void ClientSocket::forceConnectionStatusHas(bool state)
    {
      mIsConnected = state;
    }
  }
}

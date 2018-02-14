//
// Created by brd6 on 02/10/17.
//

#include "sckcpp/UdpClientSocket.hpp"

namespace sckcpp
{
  namespace udp
  {
    ClientSocket::ClientSocket(boost::asio::io_service &ios) :
     mIos(ios),
     mSocket(mIos, boost::asio::ip::udp::v4())
    {
    }

    void ClientSocket::close()
    {
      mSocket.shutdown(boost::asio::ip::udp::socket::shutdown_both);
      mSocket.close();
    }

    boost::asio::ip::udp::socket &ClientSocket::getSocket()
    {
      return mSocket;
    }

    size_t ClientSocket::sendTo(const std::string &host,
				unsigned short port,
				void const *data,
				size_t size)
    {
      boost::asio::ip::udp::endpoint ep(
       boost::asio::ip::address::from_string(host.c_str()), port);

      return mSocket.send_to(boost::asio::buffer(data, size), ep);
    }

    void ClientSocket::asyncSendTo(const std::string &host,
				   unsigned short port,
				   void const *data,
				   size_t size,
				   OnSendToFunc onSendTo)
    {
      boost::asio::ip::udp::endpoint ep(
       boost::asio::ip::address::from_string(host.c_str()), port);

      if (onSendTo != nullptr)
	mOnSendTo = onSendTo;

      mSocket.async_send_to(boost::asio::buffer(data, size), ep,
			    [=](const boost::system::error_code &ec,
				size_t ret)
			    {

			      if (mOnSendTo != nullptr)
				{
				  mOnSendTo(ret);
				}
			    });
    }

    size_t ClientSocket::receiveFrom(void *data, size_t size)
    {
      return mSocket.receive_from(boost::asio::buffer(data, size),
				  mSenderEndPoint);
    }

    void ClientSocket::asyncReceiveFrom(void *data,
					size_t size,
					OnReceiveFromFunc onReceiveFrom)
    {
      if (onReceiveFrom != nullptr)
	mOnReceiveFrom = onReceiveFrom;

      mSocket.async_receive_from(boost::asio::buffer(data, size),
				 mSenderEndPoint,
				 [=](const boost::system::error_code &ec,
				     size_t ret)
				 {

				   if (mOnReceiveFrom == nullptr)
				     return;

				   mOnReceiveFrom(
				    mSenderEndPoint.address().to_string(),
				    mSenderEndPoint.port(),
				    ret);
				 });
    }

    void
    ClientSocket::setOnReceiveFromCallback(OnReceiveFromFunc onReceiveFrom)
    {
      mOnReceiveFrom = onReceiveFrom;
    }

    void ClientSocket::setOnSendToCallback(OnSendToFunc onSendTo)
    {
      mOnSendTo = onSendTo;
    }

    std::string ClientSocket::getAddress() const
    {
      return mSocket.remote_endpoint().address().to_string();
    }

    void ClientSocket::run()
    {
      mIos.run();
    }

    void ClientSocket::stop()
    {
      mIos.stop();
    }
  }
}


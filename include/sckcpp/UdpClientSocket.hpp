//
// Created by brd6 on 28/09/17.
//


#ifndef _UdpClientSocket_H_
# define _UdpClientSocket_H_

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace sckcpp
{
  namespace udp
  {
    class ClientSocket
    {
     public:
      using OnReceiveFromFunc = std::function<void(const std::string &host,
						   unsigned short port,
						   size_t ret)>;
      using OnSendToFunc = std::function<void(size_t ret)>;

     protected:
      boost::asio::io_service &mIos;
      boost::asio::ip::udp::socket mSocket;
      OnSendToFunc mOnSendTo;
      OnReceiveFromFunc mOnReceiveFrom;
      boost::asio::ip::udp::endpoint mSenderEndPoint;

     public:
      explicit ClientSocket(boost::asio::io_service &ios);
      virtual ~ClientSocket() = default;

      void run();

      void stop();

      /**
       * Close the Client connection
       */
      void close();

      /**
       * Return the Boost asio socket
       * @return
       */
      boost::asio::ip::udp::socket &getSocket();

      /**
       * Send data
       * @param host
       * @param port
       * @param data
       * @param size
       * @return
       */
      size_t sendTo(const std::string &host,
		    unsigned short port,
		    void const *data,
		    size_t size);

      /**
       * send data asynchronously
       * @param data
       * @param size
       * @param onWrite
       */
      void asyncSendTo(const std::string &host,
		       unsigned short port,
		       void const *data,
		       size_t size,
		       OnSendToFunc onSendTo = nullptr);

      /**
       * receive data
       * @param data
       * @param size
       * @return
       */
      size_t receiveFrom(void *data,
			 size_t size);

      /**
       * receive data asynchronously
       * @param data
       * @param size
       * @param onReceiveFrom
       */
      void asyncReceiveFrom(void *data,
			    size_t size,
			    OnReceiveFromFunc onReceiveFrom = nullptr);

      /**
       * Set the callback for asyncReceiveFrom
       * @see asyncReceiveFrom
       * @param onReceiveFrom
       */
      void setOnReceiveFromCallback(OnReceiveFromFunc onReceiveFrom);

      /**
       * Set the callback for asyncSendTo
       * @see asyncSendTo
       * @param onReceiveFrom
       */
      void setOnSendToCallback(OnSendToFunc onSendTo);

      /**
       * Return the address
       * @return
       */
      std::string getAddress() const;

    };
  }
}

#endif /* !_UdpClientSocket_H_ */
//
// Created by brd6 on 28/09/17.
//


#ifndef _TcpClientSocket_H_
# define _TcpClientSocket_H_

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace sckcpp
{
  namespace tcp
  {
    using OnReadFunc = std::function<void(size_t ret)>;
    using OnWriteFunc = std::function<void(size_t ret)>;
    using OnConnectFunc = std::function<void()>;
    using OnDisconnect = std::function<void()>;

    class ClientSocket
    {
     protected:
      boost::asio::io_service &mIos;
      boost::asio::ip::tcp::socket mSocket;
      bool mIsConnected;
      OnWriteFunc mOnWrite;
      OnReadFunc mOnRead;
      OnConnectFunc mOnConnect;
      OnDisconnect mOnDisconnect;
      std::string mAddress;
      unsigned short mPort;

     public:
      explicit ClientSocket(boost::asio::io_service &ios);
      virtual ~ClientSocket() = default;

      /**
       * Connect the client to the server
       * @param host
       * @param port
       */
      void connect(std::string const &host, unsigned short port);

      /**
       * Connect the client to the server asynchronously
       * @param host
       * @param port
       */
      void asyncConnect(std::string const &host, unsigned short port,
			OnConnectFunc onConnect);

      /**
       * Close the Client connection
       */
      void close();

      /**
       * Return the Boost asio socket
       * @return
       */
      boost::asio::ip::tcp::socket &getSocket();

      /**
       * Check if the client is connect the server
       * @return
       */
      bool isConnected() const;

      /**
       * Write data
       * @param data
       * @param size
       * @return
       */
      size_t write(void const *data, size_t size);

      /**
       * Write data asynchronously
       * @param data
       * @param size
       * @param onWrite
       */
      void
      asyncWrite(void const *data, size_t size, OnWriteFunc onWrite = nullptr);

      /**
       * Read data
       * @param data
       * @param size
       * @return
       */
      size_t read(void *data, size_t size);

      /**
       * Read data asynchronously
       * @param data
       * @param size
       * @param onRead
       */
      void asyncRead(void *data, size_t size, OnReadFunc onRead = nullptr);

      /**
       * Set the callback for asyncRead
       * @see asyncRead
       * @param onRead
       */
      void setOnReadCallback(OnReadFunc onRead);

      /**
       * Set the callback for asyncWrite
       * @see asyncWrite
       * @param onWrite
       */
      void setOnWriteCallback(OnWriteFunc onWrite);

      /**
       * Set the callback for asyncConnect
       * @see asyncConnect
       * @param onConnect
       */
      void setOnConnectCallback(OnConnectFunc onConnect);

      void setOnDisconnectCallback(OnDisconnect onDisconnect);

      /**
       * Return the address
       * @return
       */
      std::string const &getAddress();

      /**
       * Return the port
       * @return
       */
      unsigned short getPort();

      void run();

      std::string getInfo();

      void forceConnectionStatusHas(bool i);

     private:
      /**
       * Helper for asyncConnect
       * @param host
       * @param port
       */
      void startAsyncConnect(std::string const &host, unsigned short port);

      /**
       * Check if the client is disconnected
       * @param ec
       * @return
       */
      bool isDisconnectRequest(const boost::system::error_code &ec);

      template<typename TCallback, typename... TArgs>
      void invokeCallback(TCallback callback, TArgs &&... args)
      {
	callback(std::forward<TArgs>(args)...);
      };

      void initializeAddressAndPort();

    };
  }
}


#endif /* !_TcpClientSocket_H_ */
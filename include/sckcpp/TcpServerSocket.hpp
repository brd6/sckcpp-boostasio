//
// Created by brd6 on 28/09/17.
//


#ifndef _TcpServerSocket_H_
# define _TcpServerSocket_H_

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include "sckcpp/Client.hpp"

namespace sckcpp
{
  namespace tcp
  {
    const int DEFAULT_BACKLOG = 10;

    using OnClientAcceptedFunc =
    std::function< void(ClientPtr client)>;

    class ServerSocket
    {
     protected:
      boost::asio::io_service &mIos;
      boost::asio::ip::tcp::acceptor mAcceptor;
      OnClientAcceptedFunc mOnClientAccepted;

     public:
      explicit ServerSocket(boost::asio::io_service &ios);

      virtual ~ServerSocket() = default;

      void bind();
      void bind(unsigned short port);
      void bind(std::string const &host, unsigned short port);

      void enableReUseAddress();

      void listen(int backlog = DEFAULT_BACKLOG);

      /*
       * Run the server
       */
      void run();

      /**
       * Stop the server
       */
      void stop();

      /**
       * Accept new client asynchronously with a callback
       * @param onClientAccepted
       */
      void asyncAccept(OnClientAcceptedFunc onClientAccepted = nullptr);

      /**
       * Accept new client
       * @return
       */
      boost::shared_ptr <Client> accept();

      /**
       * set the callback for the asyncAccept method
       * @see asyncAccept
       * @param onClientAccepted
       * @return
       */
      void setOnClientAcceptedCallback(OnClientAcceptedFunc onClientAccepted);

      std::string getInfo();

     protected:
      /**
       * Helper for callback the asyncAccept method
       */
      void startAsyncAccept();

      /**
       * Helper for create an endpoint
       * @param host
       * @param port
       * @return
       */
      boost::asio::ip::tcp::endpoint
      createEndpointFromAddress(std::string const &host, unsigned short port);

      /**
       * Return the address
       * @return
       */
      std::string getAddress() const;

      unsigned short getPort() const;
    };
  }
}

#endif /* !_TcpServerSocket_H_ */
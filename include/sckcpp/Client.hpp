//
// Created by brd6 on 26/09/17.
//

#ifndef _Client_H_
#define _Client_H_

#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <unordered_map>
#include "ClientSocket.hpp"

namespace sckcpp
{
  class Client :
	  public boost::enable_shared_from_this<Client>,
	  private boost::noncopyable
  {
   private:
    tcp::ClientSocket mSocket;
    bool mIsStopped;

   public:
    explicit Client(boost::asio::io_service& ios);

    //! Return the boost socket instance
    //! \return
    boost::asio::ip::tcp::socket &getSocket();

    void stop();
  };

  using ClientPtr = boost::shared_ptr<Client>;
  using OnStopClientFunc = std::function<void(boost::shared_ptr<Client> client)>;

  namespace helper
  {
    //! Helper for create new client
    //! \param io_service
    //! \return
    static ClientPtr createClient(boost::asio::io_service &io_service)
    {
      return boost::make_shared<Client>(io_service);
    }
  }
}

#endif //_Client_H_

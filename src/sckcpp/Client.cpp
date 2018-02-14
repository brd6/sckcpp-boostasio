//
// Created by brd6 on 26/09/17.
//

#include <iostream>
#include <boost/make_shared.hpp>
#include "sckcpp/Client.hpp"

namespace sckcpp
{
  Client::Client(boost::asio::io_service& ios) :
	  mSocket(ios),
	  mIsStopped(false)
{
    mSocket.setOnDisconnectCallback(
	    [=]() {

	      stop();

	    });
  }

  void Client::stop()
  {
    if (mIsStopped)
      return;

    mSocket.close();

    mIsStopped = true;
  }

  boost::asio::ip::tcp::socket &Client::getSocket()
  {
    return mSocket.getSocket();
  }

}

#pragma once

#include <boost/asio.hpp>
#include <string>
#include <iostream>

using boost::asio::ip::tcp;

class Client {
 public:
  Client(boost::asio::io_context& io);
  void connect(const std::string& host);
  void send(const std::string& message);
  std::string getAnswer();
 private:
  tcp::resolver resolver_;
  tcp::socket socket_;

};
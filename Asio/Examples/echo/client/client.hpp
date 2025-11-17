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
  void disconnect();
  std::string getAnswer();
 private:
  class Connection;

  tcp::resolver resolver_;
  std::shared_ptr<Connection> conn_;
  boost::asio::io_context& io_context_;
};

class Client::Connection : public std::enable_shared_from_this<Connection> {
 public:
  static std::shared_ptr<Connection> create(boost::asio::io_context& io); 

  tcp::socket& getSocket();
  void startReceiving();
 private:
  Connection(boost::asio::io_context& io);
  void handleRead(const std::string& message);
  tcp::socket socket_;
  boost::asio::streambuf buffer_;
};
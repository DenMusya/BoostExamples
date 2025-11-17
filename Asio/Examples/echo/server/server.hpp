#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

class Server {
 public:
  Server(boost::asio::io_context& io_context);
 private:
  class Connection;
  void startAccept();
  void handleAccept(std::shared_ptr<Connection> conn, boost::system::error_code err);
  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};

class Server::Connection : public std::enable_shared_from_this<Connection> {
 public:
  static std::shared_ptr<Connection> create(boost::asio::io_context& io); 

  tcp::socket& getSocket();
  void start();
 private:
  Connection(boost::asio::io_context& io);
  void handleRead(const std::string& message);
  tcp::socket socket_;
  boost::asio::streambuf buffer_;
};
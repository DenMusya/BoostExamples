#include "client.hpp"
#include <array>

std::shared_ptr<Client::Connection> Client::Connection::create(boost::asio::io_context& io) {
  return std::shared_ptr<Connection>(new Connection(io));
}

Client::Connection::Connection(boost::asio::io_context& io): socket_(io) {}

tcp::socket& Client::Connection::getSocket() {
  return socket_;
}

void Client::Connection::startReceiving() {
  auto self = shared_from_this();
  boost::asio::async_read_until(socket_, buffer_, '\n', [this, self](boost::system::error_code err, std::size_t bytes){
    if (!err) {
      std::istream is(&buffer_);
      std::string line;
      std::getline(is, line);
      handleRead(line);
      startReceiving();
    }
  });
}

void Client::Connection::handleRead(const std::string& message) {
  std::cout << "[CLIENT] Message from the server has received: " << message << std::endl;
}

Client::Client(boost::asio::io_context& io): resolver_(io), io_context_(io) {}

void Client::connect(const std::string& host) {
  conn_ = Connection::create(io_context_);
  resolver_.async_resolve(host, std::to_string(PORT), [this](boost::system::error_code err, boost::asio::ip::tcp::resolver::results_type endpoints){
    if (!err) {
      boost::asio::async_connect(conn_->getSocket(), endpoints, [this](boost::system::error_code err, boost::asio::ip::tcp::endpoint){
        if (!err) {
          conn_->startReceiving();
        }
      });
    }
  });
}

void Client::send(const std::string& message) {
  boost::asio::async_write(conn_->getSocket(), boost::asio::buffer(message), [](boost::system::error_code err, std::size_t bytes){
    if (!err) {
      std::cout << "[CLIENT] " << bytes << " bytes were sent to the server" << std::endl;
    }
  });
}

void Client::disconnect() {
  boost::system::error_code err;
  conn_->getSocket().close(err);
  if (!err) {
    std::cout << "[CLIENT] Connection was succesfully closed" << std::endl;
  }
  conn_ = nullptr;
}
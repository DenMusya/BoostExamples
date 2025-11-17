#include "server.hpp"
#include <string>

std::shared_ptr<Server::Connection> Server::Connection::create(boost::asio::io_context& io) {
  return std::shared_ptr<Connection>(new Connection(io));
}

Server::Connection::Connection(boost::asio::io_context& io): socket_(io) {}

tcp::socket& Server::Connection::getSocket() {
  return socket_;
}

void Server::Connection::start() {
  auto self = shared_from_this();
  boost::asio::async_read_until(socket_, buffer_, '\n', [this, self](boost::system::error_code err, std::size_t bytes){
    if (!err) {
      std::istream is(&buffer_);
      std::string line;
      std::getline(is, line);
      handleRead(line);
      start();
    }
  });
}

void Server::Connection::handleRead(const std::string& message) {
  std::cout << "Writing message to the host..." << std::endl;
  std::cout << message << std::endl;

  std::string to_send = message + '\n';
  boost::asio::async_write(socket_, boost::asio::buffer(to_send), [](boost::system::error_code, std::size_t){});
}



Server::Server(boost::asio::io_context& io_context): io_context_(io_context),
                                                     acceptor_(io_context_, tcp::endpoint(tcp::v4(), PORT)) {
  startAccept();
}

void Server::startAccept() {
  auto conn = Connection::create(io_context_);

  acceptor_.async_accept(conn->getSocket(), std::bind(&Server::handleAccept, this, conn, boost::asio::placeholders::error));
}

void Server::handleAccept(std::shared_ptr<Connection> conn, boost::system::error_code err) {
  if (!err) {
    std::cout << "Connected to the host." << std::endl;
    conn->start();
  }

  startAccept();
}
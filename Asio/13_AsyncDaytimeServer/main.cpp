#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <memory>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}



class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
 public:
  static std::shared_ptr<TCPConnection> create(boost::asio::io_context& io_context) {
    return std::shared_ptr<TCPConnection>(new TCPConnection(io_context));
  }

  tcp::socket& getSocket() {
    return socket;
  }

void start() {
    message = make_daytime_string();
    auto self = shared_from_this();  // создаём shared_ptr
    boost::asio::async_write(socket, boost::asio::buffer(message), std::bind(&TCPConnection::handle_write, shared_from_this()));
}

 private:
  TCPConnection(boost::asio::io_context& io_context) : socket(io_context) {}
  void handle_write() {}

  tcp::socket socket;
  std::string message;
  // boost::asio::io_context& io_context;
};

class TCPServer {
 public:
  TCPServer(boost::asio::io_context& io_context): io_context(io_context), acceptor(io_context, tcp::endpoint(tcp::v4(), 13)) {
    start_accept();
  }

  void start_accept() {
    auto new_connection =
      TCPConnection::create(io_context);

    acceptor.async_accept(new_connection->getSocket(),
        std::bind(&TCPServer::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }
 private:

  void handle_accept(std::shared_ptr<TCPConnection> conn, const boost::system::error_code& error) {
    if (!error)
    {
      std::cout << "New client connected!" << std::endl;
      conn->start();
    }
    start_accept();
  }

  boost::asio::io_context& io_context;
  tcp::acceptor acceptor;
};

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;
    TCPServer server(io_context);
    
    io_context.run();
    
  } catch (std::exception& e) {
    std::cerr << e.what();
    return 1;
  }

  return 0;
}
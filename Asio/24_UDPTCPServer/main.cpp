#include<iostream>
#include<boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class UDPServer {
 public:
  UDPServer(boost::asio::io_context& io_context): socket(io_context, udp::endpoint(udp::v4(), 13)) {
    start_receiving();
  }
 private:
  void start_receiving() {
    socket.async_receive_from(boost::asio::buffer(recv_buf), sender_endpoint, 
                              std::bind(&UDPServer::receive_handler, this, 
                                        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }

  void receive_handler(boost::system::error_code error, size_t bytes_transferred) {
    if (!error) {
      auto message = std::make_shared<std::string>(make_daytime_string());
      socket.async_send_to(boost::asio::buffer(*message), sender_endpoint, 
                          std::bind(&UDPServer::send_handler, this,
                                    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
      start_receiving();
    }
  }

  void send_handler(boost::system::error_code error, size_t bytes_transferred) {

  }

  udp::socket socket;
  udp::endpoint sender_endpoint;
  std::array<char, 1> recv_buf;
};

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

int main() {
  try {
    boost::asio::io_context io_context;
    
    TCPServer tcp_server(io_context);
    UDPServer udp_server(io_context);

    io_context.run();

  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
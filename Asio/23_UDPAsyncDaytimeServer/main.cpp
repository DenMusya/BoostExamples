#include <iostream>
#include <boost/asio.hpp>
#include <array>
#include <memory>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std;
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

int main() {
  try {
    boost::asio::io_context io_context;
    UDPServer server(io_context);
    io_context.run();
  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
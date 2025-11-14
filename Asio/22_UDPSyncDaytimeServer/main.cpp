#include <iostream>
#include <boost/asio.hpp>
#include <array>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
  using namespace std;
  time_t now = time(0);
  return ctime(&now);
}

int main() {
  try {
    boost::asio::io_context io_context;

    udp::socket socket(io_context, udp::endpoint(udp::v4(), 13));

    while (true) {
      std::array<char, 1> recv_buffer;

      udp::endpoint sender_endpoint;
      socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);

      std::string str_to_send = make_daytime_string();

      boost::system::error_code ignored_error;
      socket.send_to(boost::asio::buffer(str_to_send), sender_endpoint, 0, ignored_error);
    }

  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 228;
  }
  return 0;
}

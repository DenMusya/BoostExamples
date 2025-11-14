#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[]) {
  try {
    if (argc < 2) {
      std::cerr << "Need at least 2 args\n"; 
      return 1;
    }
    
    boost::asio::io_context io_context;

    udp::resolver resolver(io_context);
    udp::endpoint endpoint = *resolver.resolve(udp::v4(), argv[1], "daytime").begin();

    udp::socket socket(io_context);
    socket.open(udp::v4());

    std::array<char, 1> buffer = {1};

    socket.send_to(boost::asio::buffer(buffer), endpoint);
    
    std::array<char, 128> recv_buffer;
    udp::endpoint sender_endpoint;

    std::size_t len = socket.receive_from(boost::asio::buffer(recv_buffer), sender_endpoint);
    std::cout.write(recv_buffer.data(), len);
  } catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
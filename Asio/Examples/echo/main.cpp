#include <iostream>
#include "server.hpp"
#include "client.hpp"
#include <boost/asio.hpp>

int main() {
  std::cout << "Type 1 to become server\nType 2 to become host" << std::endl;
  std::string str;
  std::cin >> str;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  
  if (str == "1") {
    boost::asio::io_context io;
    Server server(io);
    io.run();
  } else if (str == "2") {
    boost::asio::io_context io;
    Client client(io);
    client.connect("localhost");
    while(true) {
      //client.connect("localhost");
      std::string message;
      std::cout << "Write anything: ";
      std::getline(std::cin, message);
      client.send(message + '\n');
      std::cout << "Server response: " << client.getAnswer() << std::endl;
    }

  } else {
    std::cerr << "Error input" << std::endl;
    return 1;
  }

  return 0;
}
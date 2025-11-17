#include <iostream>
#include "server.hpp"
#include "client.hpp"
#include <boost/asio.hpp>
#include <thread>

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
    bool running = true;
    boost::asio::io_context io;
    auto work = boost::asio::make_work_guard(io);
    Client client(io);
    std::thread thr([&io](){io.run();});
    while (running) {
      char command;
      std::cout << "Choose command:\n[1] Connect\n[2] Send message\n[3] Disconnect\n[4] Exit" << std::endl;
      std::cout << "Command: ";
      std::cin >> command;
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      switch (command) {
      case '1': {
        std::string host;
        std::cout << "Write server host: ";
        std::cin >> host;
        client.connect(host);
        break;
      }
      case '2': {
        std::string message;
        std::cout << "Write anything: ";
        std::getline(std::cin, message);
        client.send(message + '\n');
        break;
      }
      case '3':
        client.disconnect();
        break;
      case '4':
        client.disconnect();
        running = false;
        break;
      default:
        break;
      }
    }

    work.reset();
    thr.join();
  } else {
    std::cerr << "Error input" << std::endl;
    return 1;
  }

  return 0;
}
#include <iostream>
#include <boost/asio.hpp>

void print(const boost::system::error_code& e) {
  std::cout << "Timer for 5 seconds has ended" << std::endl;
}

int main()
{
  boost::asio::io_context io;

  
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
  std::cout << "Timer for 5 seconds started" << std::endl;
  t.async_wait(&print);
  std::cout << "Execution continues immediately after async_wait" << std::endl;

  io.run();
  return 0;
}
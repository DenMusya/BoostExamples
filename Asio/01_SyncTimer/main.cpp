#include <iostream>
#include <boost/asio.hpp>

int main()
{
  boost::asio::io_context io;

  
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
  std::cout << "Timer for 5 seconds started" << std::endl;
  t.wait();

  std::cout << "Timer for 5 seconds has ended" << std::endl;

  return 0;
}
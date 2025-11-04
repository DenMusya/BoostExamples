#include <iostream>
#include <functional>
#include <boost/asio.hpp>

void print(const boost::system::error_code& e, boost::asio::steady_timer* t, int* count) {
  std::cout << "Timer has ended" << std::endl;

  if (*count < 5) {
    ++*count;
    t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
    std::cout << "Timer for 1 second started for " << *count + 1 << " time" << std::endl;
    t->async_wait(std::bind(print, boost::asio::placeholders::error, t, count));
  }
}

int main()
{
  boost::asio::io_context io;

  int count = 0;
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
  std::cout << "Timer for 1 second started for 1 time" << std::endl;
  t.async_wait(std::bind(print, boost::asio::placeholders::error, &t, &count));

  io.run();
  return 0;
}
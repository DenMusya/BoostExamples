#include <iostream>
#include <functional>
#include <boost/asio.hpp>

class Printer {
public:
  Printer(boost::asio::io_context& io): t(io, boost::asio::chrono::seconds(1)), count(0) {
    t.async_wait(std::bind(&Printer::print, this));
    std::cout << "Timer for 1 second started for 1 time" << std::endl;
  } 
private:
  void print() { // no need for boost::system::error_code
    std::cout << "Timer has ended" << std::endl;

    if (count < 5) {
      ++count;
      t.expires_at(t.expiry() + boost::asio::chrono::seconds(1));
      std::cout << "Timer for 1 second started for " << count + 1 << " time" << std::endl;
      t.async_wait(std::bind(&Printer::print, this));
    }
  }

  boost::asio::steady_timer t;
  int count;
};

int main()
{
  boost::asio::io_context io;

  Printer printer(io);

  io.run();
  return 0;
}
#include <iostream>
#include <functional>
#include <boost/asio.hpp>
#include <thread>

class Printer {
public:
  Printer(boost::asio::io_context& io): t1(io, boost::asio::chrono::seconds(1)), t2(io, boost::asio::chrono::seconds(1)),
          strand(boost::asio::make_strand(io)), count(0) {
    
    t1.async_wait(boost::asio::bind_executor(strand, std::bind(&Printer::print1, this)));
    t2.async_wait(boost::asio::bind_executor(strand, std::bind(&Printer::print2, this)));

    std::cout << "1st timer for 1 second started. Count = " << count << std::endl;
    std::cout << "2nd timer for 1 second started. Count = " << count << std::endl;
  } 
private:
  void print1() { 
    std::cout << "1st timer has ended" << std::endl;

    if (count < 10) {
      ++count;
      t1.expires_at(t1.expiry() + boost::asio::chrono::seconds(1));
      std::cout << "1st timer for 1 second started. Count = " << count << std::endl;
      t1.async_wait(boost::asio::bind_executor(strand, std::bind(&Printer::print1, this)));
    }
  }

  void print2() { 
    std::cout << "2nd timer has ended" << std::endl;

    if (count < 10) {
      ++count;
      t2.expires_at(t2.expiry() + boost::asio::chrono::seconds(1));
      std::cout << "2nd timer for 1 second started. Count = " << count << std::endl;
      t2.async_wait(boost::asio::bind_executor(strand, std::bind(&Printer::print2, this)));
    }
  }

  boost::asio::steady_timer t1;
  boost::asio::steady_timer t2;
  boost::asio::strand<boost::asio::io_context::executor_type> strand;
  int count;
};

int main()
{
  boost::asio::io_context io;

  Printer printer(io);

  std::thread thread1([&](){ io.run(); });
  io.run();

  thread1.join();
  return 0;
}
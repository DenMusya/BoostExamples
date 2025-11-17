#include "client.hpp"
#include <array>

Client::Client(boost::asio::io_context& io): resolver_(io), socket_(io) {}

void Client::connect(const std::string& host) {
  auto endpoints = resolver_.resolve(host, std::to_string(PORT));
  boost::asio::connect(socket_, endpoints);
}

void Client::send(const std::string& message) {
  boost::system::error_code error;
  boost::asio::write(socket_, boost::asio::buffer(message), error);
  if (error) {
    std::cerr << "Error during write: " << error.message() << std::endl;
  }
}

std::string Client::getAnswer() {
  std::string answer;
  boost::asio::streambuf buffer;
  boost::asio::read_until(socket_, buffer, '\n');
  std::istream is(&buffer);
  std::getline(is, answer);
  return answer;
}
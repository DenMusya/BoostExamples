cmake_minimum_required(VERSION 4.1)
project(Asio_24_UDPTCPServer)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} boost_project)
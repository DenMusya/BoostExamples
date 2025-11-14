cmake_minimum_required(VERSION 4.1)
project(Asio_23_UDPAsyncDaytimeServer)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} boost_project)
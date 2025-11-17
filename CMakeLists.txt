cmake_minimum_required(VERSION 4.1)
project(BoostExamples)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_policy(SET CMP0167 NEW)

find_package(Boost 1.89 REQUIRED)

add_library(boost_project INTERFACE)
target_include_directories(boost_project INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(boost_project INTERFACE ${Boost_LIBRARIES})


add_subdirectory(Asio)

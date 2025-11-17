cmake_minimum_required(VERSION 4.1)
project(echo)

set(PORT 8080)

add_subdirectory(client)
add_subdirectory(server)

target_compile_definitions(server PUBLIC PORT=${PORT})
target_compile_definitions(client PUBLIC PORT=${PORT})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
      server
      client
      boost_project)


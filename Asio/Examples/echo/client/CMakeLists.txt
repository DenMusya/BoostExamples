add_library(client client.cpp)

target_include_directories(client
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(client boost_project)
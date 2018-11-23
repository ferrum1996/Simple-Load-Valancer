cmake_minimum_required(VERSION 3.12)
project(Simple_Load_Valancer)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost COMPONENTS system filesystem REQUIRED)

add_executable(Simple_Load_Valancer main.cpp tcp_server.cpp tcp_server.h tcp_connection.cpp tcp_connection.h)
target_link_libraries(Simple_Load_Valancer pthread)
target_link_libraries(Simple_Load_Valancer ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
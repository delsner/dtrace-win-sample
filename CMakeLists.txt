cmake_minimum_required(VERSION 3.17)
project(syscall_samples)

set(CMAKE_CXX_STANDARD 17)

add_executable(syscall_file_read file_read.cpp)
add_executable(syscall_socket_read socket_read.cpp)
add_executable(syscall_create_process create_process.cpp)
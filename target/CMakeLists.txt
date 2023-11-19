cmake_minimum_required(VERSION 3.22)
project(blinky)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

add_executable(blinky main.cpp Handler.cpp)

add_subdirectory(hardware)
add_subdirectory(communication)

target_compile_options(blinky PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(blinky PRIVATE gpiod gpiodcxx pthread)
cmake_minimum_required(VERSION 3.22)
project(blinky)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

add_executable(blinky
    main.cpp
    LED.cpp
    Button.cpp
    DS18B20.cpp
    Blinker.cpp
    Protocol.cpp
    Server.cpp
    Handler.cpp
)

target_compile_options(blinky PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(blinky PRIVATE gpiod gpiodcxx pthread)
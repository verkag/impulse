cmake_minimum_required(VERSION 3.15) 
project(impulse)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangdb  

add_executable(app src/main.cpp src/server.cpp src/event.cpp src/parser.cpp)
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/src)


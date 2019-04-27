cmake_minimum_required(VERSION 3.8)
project(protocol-testbench
  VERSION 0.1.0)

find_package(Boost REQUIRED COMPONENTS
  program_options)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/header_only/)

add_subdirectory(testbench)
add_subdirectory(tester)

set(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/testbench.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test-results.cpp
  )

add_subdirectory(protocols)

add_library(testbench SHARED ${SRCS} ${PROTOCOLS_SRCS})
set_property(TARGET testbench PROPERTY CXX_STANDARD 17)

target_include_directories(testbench PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(testbench PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

configure_file("testbench-config.hpp.in" "testbench-config.hpp")

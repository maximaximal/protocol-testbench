set(SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_executable(tester ${SRCS})
set_property(TARGET tester PROPERTY CXX_STANDARD 17)

target_link_libraries(tester testbench Boost::program_options)

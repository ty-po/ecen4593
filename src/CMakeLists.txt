cmake_minimum_required (VERSION 2.9)
project (ecen4593final)

set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

add_executable(ecen4593final
  main.cc
  config.cc
  simulator.cc
  output.cc
  cache.cc
)

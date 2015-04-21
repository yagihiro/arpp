cmake_minimum_required(VERSION 3.0)

add_definitions("-Wall -std=c++11")
include_directories(
  include
	vendor/cppformat
	vendor/SQLiteCpp/include
)
add_subdirectory(src)
add_subdirectory(vendor/cppformat)
add_subdirectory(vendor/SQLiteCpp)
add_subdirectory(test)

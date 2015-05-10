cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall -std=c++0x")
include_directories(
  include
	vendor/cppformat
	vendor/SQLiteCpp/include
)

find_package (Threads)

add_subdirectory(src)
add_subdirectory(vendor/cppformat)
add_subdirectory(vendor/SQLiteCpp)
add_subdirectory(test)

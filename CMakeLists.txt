cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall")
include_directories(
  include
	vendor/cppformat
	vendor/SQLiteCpp/include
)
add_subdirectory(src)
add_subdirectory(vendor/cppformat)
add_subdirectory(vendor/SQLiteCpp)
add_subdirectory(test)

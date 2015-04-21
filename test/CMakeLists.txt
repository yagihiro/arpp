cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall -std=c++11")
include_directories(
  ../include
	../vendor/Catch/include
	../vendor/cppformat
	../vendor/SQLiteCpp/include
)
add_executable(testrunner
  build.cpp
)
target_link_libraries(testrunner arpp format SQLiteCpp sqlite3)

cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall -std=c++0x")
include_directories(
  ../include
	../vendor/Catch/include
	../vendor/cppformat
	../vendor/SQLiteCpp/include
)

find_package (Threads)

add_executable(testrunner
  build.cpp
  connection-test.cpp
)
target_link_libraries(testrunner arpp format SQLiteCpp sqlite3 ${CMAKE_THREAD_LIBS_INIT})

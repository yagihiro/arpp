cmake_minimum_required(VERSION 2.8.7)

add_definitions("-Wall -std=c++11")
include_directories(
  ../include
	../vendor/cppformat
	../vendor/SQLiteCpp/include
)
add_library(arpp STATIC
	base.cpp
  connection.cpp
	migration.cpp
	schema.cpp
	status.cpp
)

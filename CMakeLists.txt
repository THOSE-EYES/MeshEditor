#Setting CMake
cmake_minimum_required(VERSION 3.15)
project(MeshEditor)

# Setting the compiler for Windows
if (WIN32)
	#set(CXXFLAGS "")
	#set(CMAKE_CXX_STANDARD_LIBRARIES "-lopenal -lboost_serialization -lgtest")
endif (WIN32)

# Setting the compiler for Linux
if (UNIX)
	set(CMAKE_CXX_COMPILER "g++")
	set(CXXFLAGS "")
	set(CMAKE_CXX_STANDARD_LIBRARIES "-lpthread -lopenal -lboost_serialization -lgtest")
endif (UNIX)

# Looking for source files
include_directories(include/MeshEditor)

# Find the files of the project
file(GLOB SOURCES "src/*.cpp")

# Command to compile the whole project
add_executable(MeshEditor ${SOURCES})
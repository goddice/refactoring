# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)

project ("refactoring")
set(CMAKE_CXX_STANDARD 20)

# Add Catch2 
add_subdirectory("thirdparty/Catch2")

# Add third party header libs
set(THIRDPARTY_PATH 
	"${CMAKE_SOURCE_DIR}/thirdparty/nlohmann/single_include"
)
include_directories(${THIRDPARTY_PATH})

message(${CMAKE_BINARY_DIR})
file(COPY "${CMAKE_SOURCE_DIR}/res" DESTINATION ${CMAKE_BINARY_DIR})

# Include sub-projects.
add_subdirectory ("chap1")
add_subdirectory ("chap4")
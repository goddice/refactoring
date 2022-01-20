# CMakeList.txt : CMake project for refactoring, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

# Add source to this project's executable.
add_executable (chap4 "chap4.cpp" "production.h" "production.cpp")
target_link_libraries(chap4 PRIVATE Catch2::Catch2WithMain)

# TODO: Add tests and install targets if needed.

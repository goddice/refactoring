# CMakeList.txt : CMake project for refactoring, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

# Add source to this project's executable.
add_executable (chap1_naive "chap1_naive.cpp")
add_executable (chap1_func "chap1_func.cpp")
add_executable (chap1_split "chap1_split.cpp" )


# TODO: Add tests and install targets if needed.

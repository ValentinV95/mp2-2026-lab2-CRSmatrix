cmake_minimum_required(VERSION 3.14)

project(MatrixLab)
include_directories(include gtest)

# BUILD
add_subdirectory(samples)
add_subdirectory(test)
add_subdirectory(gtest)
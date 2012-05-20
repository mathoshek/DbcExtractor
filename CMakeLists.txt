project(DbcExtractor)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/bin")

add_subdirectory(dep/StormLib)
add_subdirectory(src)


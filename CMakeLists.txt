cmake_minimum_required(VERSION 3.16)
project(Manager)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(Manager manager.cpp)

set(CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_NAME Manager)
set(CPACK_PACKAGE_VENDOR "jimping")
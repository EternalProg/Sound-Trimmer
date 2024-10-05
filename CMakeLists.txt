cmake_minimum_required(VERSION 3.20)
project(Sound-Trimmer)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)

add_executable(Sound-Trimmer main.cpp)

include_directories(${SNDFILE_INCLUDE_DIRS})
link_directories(${SNDFILE_LIBRARY_DIRS})
target_link_libraries(Sound-Trimmer PRIVATE ${SNDFILE_LIBRARIES})
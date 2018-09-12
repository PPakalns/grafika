cmake_minimum_required(VERSION 2.8)

project(DATZ3073)
find_package(OpenCV REQUIRED )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# DisplayImage example from OpenCV
add_subdirectory(test)


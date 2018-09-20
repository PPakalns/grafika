cmake_minimum_required(VERSION 3.10)

project(DATZ3073)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
# find_package(GLUT REQUIRED)
find_package(Gnuplot REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang"
   OR CMAKE_COMPILER_IS_GNUCC
   OR CMAKE_COMPILER_IS_GNUCXX)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
        -O2 -Wall -pedantic  \
        -pedantic-errors -Wextra -Wcast-align \
        -Wcast-qual \
        -Wdisabled-optimization \
        -Werror -Wfloat-equal -Wformat=2 \
        -Wformat-nonliteral -Wformat-security  \
        -Wformat-y2k \
        -Wimport  -Winit-self  -Winline \
        -Winvalid-pch   \
        -Wlong-long \
        -Wmissing-field-initializers -Wmissing-format-attribute   \
        -Wmissing-include-dirs -Wmissing-noreturn \
        -Wpacked  -Wpadded -Wpointer-arith \
        -Wredundant-decls \
        -Wshadow -Wstack-protector \
        -Wstrict-aliasing=2 -Wswitch-default \
        -Wswitch-enum \
        -Wunreachable-code -Wunused \
        -Wunused-parameter -Wunused-value\
        -Wunused-function \
        -Wunused-label \
        -Wunused-value \
        -Wunused-variable \
        -Wvariadic-macros \
        -Wwrite-strings")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
        -pedantic -Wall -Wextra -Wwrite-strings -Werror")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -fsanitize=address,leak")
endif()

# Dummy target for compile argument database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_target(compile_commands ALL
    DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMAND cp ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}
    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
    )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# DisplayImage example from OpenCV
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(core)
add_subdirectory(glwrap)
add_subdirectory(test)
add_subdirectory(1A)
add_subdirectory(third_party)
add_subdirectory(2_1B)
add_subdirectory(2_2A)


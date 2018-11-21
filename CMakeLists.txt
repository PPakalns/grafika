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
        -Wmissing-field-initializers -Wmissing-format-attribute   \
        -Wmissing-include-dirs -Wmissing-noreturn \
        -Wpacked  -Wpointer-arith \
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
if(NOT WIN32)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

	add_custom_target(compile_commands ALL
		DEPENDS ${CMAKE_SOURCE_DIR}/compile_commands.json)

	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/compile_commands.json
		COMMAND cp ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}
		DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
		)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
add_subdirectory(core)
add_subdirectory(glwrap)
add_subdirectory(1A)
add_subdirectory(third_party)
add_subdirectory(2_1B)
add_subdirectory(2_2A)
add_subdirectory(3D)
add_subdirectory(4A)
add_subdirectory(8B)

# DisplayImage example from OpenCV
add_subdirectory(test)

if(WIN32)
add_custom_target(dll_fixup ALL)
add_custom_command(TARGET dll_fixup POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${OpenCV_DIR}/x64/vc15/bin/opencv_world343.dll
		${glfw3_DIR}/../../glfw3.dll
		${GLEW_DIR}/glew32.dll
		${CMAKE_SOURCE_DIR}/1A/1a.py
		"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>")
endif()

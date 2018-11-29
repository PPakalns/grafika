set(SOURCES
    main.cpp)

add_executable(9a ${SOURCES})
target_link_libraries(9a core ${OpenCV_LIBS})

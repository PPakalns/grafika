set(SOURCES
    main.cpp)

add_executable(1a ${SOURCES})
target_link_libraries(1a core ${OpenCV_LIBS})

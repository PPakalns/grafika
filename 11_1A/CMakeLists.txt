set(SOURCES
    main.cpp)

add_executable(11_1a ${SOURCES})
target_link_libraries(11_1a core ${OpenCV_LIBS})

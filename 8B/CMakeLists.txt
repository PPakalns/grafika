set(SOURCES
    main.cpp)

add_executable(8b ${SOURCES})
target_link_libraries(8b core ${OpenCV_LIBS})

set(SOURCES
    main.cpp)

add_executable(DisplayImage ${SOURCES})
target_link_libraries(DisplayImage core ${OpenCV_LIBS})

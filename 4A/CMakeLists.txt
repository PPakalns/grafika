set(SOURCES
    main.cpp)

add_executable(4a ${SOURCES})
target_link_libraries(4a core ${OpenCV_LIBS})
# target_link_libraries(1a core ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

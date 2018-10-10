set(SOURCES
    main.cpp)

add_executable(2_2a ${SOURCES})
target_link_libraries(2_2a core ${OpenCV_LIBS})
# target_link_libraries(1a core ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
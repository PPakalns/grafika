set(SOURCES
    main.cpp)

add_executable(3d ${SOURCES})
target_link_libraries(3d core ${OpenCV_LIBS})
# target_link_libraries(1a core ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

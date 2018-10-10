set(SOURCES
    main.cpp)

add_executable(2_1b ${SOURCES})
target_link_libraries(2_1b core glwrap ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${GLM_LIBRARIES})
# target_link_libraries(1a core ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
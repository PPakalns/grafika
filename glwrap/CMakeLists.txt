set(SOURCES
    Utility.h
    Utility.cpp)

add_library(glwrap STATIC ${SOURCES})
target_link_libraries(glwrap core ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw ${GLM_LIBRARIES})

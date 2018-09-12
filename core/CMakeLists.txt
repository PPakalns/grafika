set(SOURCES
    Utility.h
    Utility.cpp)

add_library(core STATIC ${SOURCES})
target_link_libraries(core ${OpenCV_LIBS})

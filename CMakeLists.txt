cmake_minimum_required(VERSION 3.14)
project(t1)

set(CMAKE_CXX_STANDARD 14)

add_executable(t1 src/main.cpp src/configs.h)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${PNG_INCLUDE_DIR} )

target_link_libraries(t1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${PNG_LIBRARIES} )

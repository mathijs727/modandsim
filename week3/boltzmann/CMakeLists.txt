cmake_minimum_required(VERSION 3.3)
project(boltzmann)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    boltzmann.cpp
    ErrorHandling.h
    ShaderProgram.cpp
    ShaderProgram.h
    Texture.cpp
    Texture.h
    Vertex.h
    VertexBuffer.cpp
    VertexBuffer.h
    Window.cpp
    Window.h)

include_directories(libs/include GLFW glm)

find_package( GLEW REQUIRED )
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
if (APPLE)
    find_package( OPENGL REQUIRED )
else(APPLE)
    if (WIN32)
        find_package( OPENGL REQUIRED )
    endif(WIN32)
endif(APPLE)
add_executable( boltzmann ${SOURCE_FILES} )
target_link_libraries( boltzmann ${GLEW_LIBRARY} ${OPENGL_LIBRARY} ${GLFW_LIBRARIES} )
cmake_minimum_required(VERSION 3.2)
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
        Window.h
        Sequential.cpp
        Sequential.h BoltzmannGrid2D9Q.h BoltzmannGrid2D9Q.cpp BoltzmannGrid2D9Q.cpp.h)

include_directories(libs/include glm)

find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

add_executable(boltzmann ${SOURCE_FILES})
target_link_libraries(boltzmann ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_STATIC_LIBRARIES})
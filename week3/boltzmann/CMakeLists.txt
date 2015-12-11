cmake_minimum_required(VERSION 3.2)
project(boltzmann)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

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
    FPSCounter.cpp
    FPSCounter.h
    BoltzmannGridD2Q9.cpp
    BoltzmannGridD2Q9.h
    Defines.h
    ImageShape.cpp
    ImageShape.h)

include_directories(libs/include glm)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(FreeImage REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${FREEIMAGE_INCLUDE_DIRS})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   # Mac OS X fucked up overnight
   link_directories(/usr/local/lib)
   # Clang doesnt support OpenMP so use gcc 5 on OS X
   set(CMAKE_CXX_COMPILER "g++-mp-5")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 


add_executable(boltzmann-openmp ${SOURCE_FILES})
target_compile_definitions(boltzmann-openmp PRIVATE OPENMP_ENABLED=1)
target_link_libraries(boltzmann-openmp ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${FREEIMAGE_LIBRARIES})


add_executable(boltzmann-sequential ${SOURCE_FILES})
target_link_libraries(boltzmann-sequential ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} ${GLFW_STATIC_LIBRARIES} ${FREEIMAGE_LIBRARIES})
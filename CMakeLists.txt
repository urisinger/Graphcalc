cmake_minimum_required(VERSION 3.25)
project(Graphcalc_cmake)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers)

add_executable(Graphcalc_cmake src/Main.cpp src/IndexBuffer.cpp src/Parser.cpp src/VertexBuffer.cpp src/shader.cpp src/Application.cpp src/glad.c)

find_package(GLEW REQUIRED)
find_path(GLEW_INCLUDE_DIR GL/glew.h)

target_link_libraries(Graphcalc_cmake glfw GLEW::GLEW)
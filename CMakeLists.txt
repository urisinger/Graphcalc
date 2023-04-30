cmake_minimum_required(VERSION 3.22)
project(Graphcalc_cmake)

set(CMAKE_CXX_STANDARD 17)

    

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers ${CMAKE_CURRENT_SOURCE_DIR}/libs/includes)

add_executable(Graphcalc_cmake src/Main.cpp src/IndexBuffer.cpp src/Parser.cpp src/VertexBuffer.cpp src/shader.cpp src/Application.cpp libs/glad/glad.c src/FrameBuffer.cpp libs/imgui/imgui.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_impl_glfw.cpp libs/imgui/imgui_impl_opengl3.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp)


target_link_libraries(Graphcalc_cmake C:/dev/Graphcalc_cmake/Graphcalc_cmake/libs/lib-vc2022/glfw3.lib)


cmake_minimum_required(VERSION 3.10.0)
project(hellow_world VERSION 1.0.0)

find_package(OpenGL REQUIRED)


add_executable(hello_window 
        src/main.cpp 
        src/glad.c
        src/object.cpp
        src/stb_image.cpp)

#when searching for include file, it also goes for the directory "extern" for searching
target_include_directories(hello_window PRIVATE extern extern/glm)
target_link_libraries(hello_window PRIVATE ${CMAKE_SOURCE_DIR}/extern/GLFW/lib-mingw-w64/libglfw3.a)
target_link_libraries(hello_window PRIVATE opengl32)



cmake_minimum_required(VERSION 2.8)

project(angine)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS -std=c++11)

include_directories(../src/include/)
link_directories(../src/libs/)

add_executable(angine
	../src/main.cpp
	../src/application.cpp
	../src/window.cpp
	../src/camera.cpp
	../src/utils/objLoader.cpp
	../src/mesh.cpp
	../src/generators/gridmesh.cpp
	../src/generators/spheremesh.cpp
	../src/shader.cpp
	../src/texture.cpp
	../src/object.cpp
)


target_link_libraries(angine -lassimp)
target_link_libraries(angine -lGL)
target_link_libraries(angine -lglfw)
target_link_libraries(angine -lGLEW)


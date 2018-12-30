cmake_minimum_required(VERSION 2.8)


project(angine)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS -std=c++11)

add_library(PCHLib
	../src/pch.cpp
)

include_directories(../src/include/)
include_directories(/usr/local/include)
include_directories(../src/vendor/imgui/)

link_directories(../src/libs/)


add_executable(angine
	../src/main.cpp
	../src/window.cpp
	
	../src/vendor/imgui/imgui_impl_opengl3.cpp
	../src/vendor/imgui/imgui_impl_glfw.cpp
	
	../src/vendor/imgui/imgui.cpp
	../src/vendor/imgui/imgui_demo.cpp
	../src/vendor/imgui/imgui_draw.cpp
	../src/vendor/imgui/imgui_widgets.cpp
	
	../src/vendor/stb_image.cpp

	
	#~ ../src/UI/ui_base.cpp
	#~ 
	#~ ../src/UI/ui_container.cpp
	#~ ../src/UI/ui_base_item.cpp
	#~ ../src/UI/ui_button.cpp
	#~ 
	#~ ../src/UI/bitmap_string.cpp
	
	../src/application.cpp
	
	../src/camera.cpp
	../src/shader.cpp
	../src/texture.cpp
	../src/object.cpp
	
	../src/utils/objLoader.cpp
	
	../src/mesh.cpp
	../src/generators/mesh_generator.cpp
	../src/generators/gridmesh.cpp
	../src/generators/box_mesh.cpp
	../src/generators/file_mesh.cpp
	../src/generators/spheremesh.cpp
	../src/generators/geosphere_mesh.cpp
	
)


target_link_libraries(angine PCHLib)
target_link_libraries(angine -lassimp)
target_link_libraries(angine -lGL)
target_link_libraries(angine  -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp )
target_link_libraries(angine -lGLEW)
target_link_libraries(angine -lX11)



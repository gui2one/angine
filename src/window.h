#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"

#include "object.h"
#include "shader.h"
#include "camera.h"
#include "generators/mesh_generator.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Window
{
	public:
		Window();
		virtual ~Window();
		void refresh();
		
		GLFWwindow * win;
		Camera camera;
		
		int width, height;
		bool shouldClose();
		
		void addObject(Object* obj);
		std::vector<Object*> objects;
		void renderObjects();

		Shader pointShader;
		
		//~ unsigned int shader_id;
		
		glm::mat4 ModelViewProjectionMatrix;
		GLuint PROJECTIONLoc, MODEL_LOC;
		
		
		int explorer();
		std::string current_explorer_path = "/";
		
		void addPropertiesDialog();
		void addObjectListDialog();
		int cur_object_selected = 0;
	private:
		
		
		/* add your private declarations */
};

#endif /* WINDOW_H */ 

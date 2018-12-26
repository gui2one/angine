#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"

#include "object.h"
#include "shader.h"
#include "camera.h"

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
		void addDialog();
		GLFWwindow * win;
		Camera camera;
		
		int width, height;
		bool shouldClose();
		
		std::vector<Object*> objects;
		void renderObjects();

		
		unsigned int shader_id;
		
		glm::mat4 ModelViewProjectionMatrix;
		GLuint PROJECTIONLoc, MODEL_LOC;
	private:
		
		
		/* add your private declarations */
};

#endif /* WINDOW_H */ 

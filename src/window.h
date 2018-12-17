#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"

#include "object.h"
#include "shader.h"
#include "camera.h"
#include "UI/ui_base.h"
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
		
		std::vector<Object*> objects;
		void renderObjects();
		UIBase UI;
		void renderUI();
		
		unsigned int shader_id;
		
		glm::mat4 ModelViewProjectionMatrix;
		GLuint PROJECTIONLoc, MODEL_LOC;
	private:
		
		
		/* add your private declarations */
};

#endif /* WINDOW_H */ 

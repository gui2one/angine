#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"
//~ #include <GL/glew.h>
//~ #include <GLFW/glfw3.h>
//~ 
//~ #include "vendor/glm/glm.hpp"
//~ #include "vendor/glm/gtc/matrix_transform.hpp"
//~ #include "vendor/glm/gtc/type_ptr.hpp"

#include "object.h"
#include "shader.h"
class Window
{
	public:
		Window();
		virtual ~Window();
		void refresh();
		
		GLFWwindow * win;
		
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

#ifndef APPLICATION_H
#define APPLICATION_H


#include <vector>
#include "pch.h"
#include "window.h"
#include "object.h"
#include "shader.h"
#include "utils/objLoader.h"


class Application
{
	public:
		Application();
		virtual ~Application();
		
		Window  window;

		void printObjectsInfos();
		
		void addObject(Object* obj);
		std::vector<Object*> objects;
		
		//~ static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		//~ static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
		//~ static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	private:
		
		/* add your private declarations */
};

#endif /* APPLICATION_H */ 

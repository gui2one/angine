#ifndef APPLICATION_H
#define APPLICATION_H



#include "pch.h"
#include "window.h"

#include "imgui_impl_glfw.h"

class Application
{
	public:
		Application();
		virtual ~Application();
		
		Window  window;

		//~ static void char_mods_callback(GLFWwindow* window, unsigned int key, int action);
		//~ static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
		//~ static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	private:
		
		/* add your private declarations */
};

#endif /* APPLICATION_H */ 

#include "application.h"
#include <iostream>





Application::Application()
{
	std::cout <<  "Creating New Application\n";
	
	glfwSetWindowUserPointer(window.win, this);
	


	glfwSetCharModsCallback(window.win, char_mods_callback);
	
	
	//~ glfwSetCursorPosCallback(window.win, cursor_pos_callback);
	//~ glfwSetMouseButtonCallback(window.win, mouse_button_callback);
	//~ glfwSetScrollCallback(app.window.win, scroll_callback);
}


void Application::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	
	
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
     
}

void Application::char_mods_callback(GLFWwindow* window, unsigned int key, int mod)
{
	if( mod == GLFW_MOD_SHIFT)
	{
		printf("shift + %c\n", std::tolower((char)key));
	}
}



Application::~Application()
{	
	std::cout <<  "Deleting Application\n";
}





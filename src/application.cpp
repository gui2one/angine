#include "application.h"
#include <iostream>

void Application::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if(key == GLFW_KEY_UP && action== GLFW_PRESS)
		std::cout << "KEY UP\n";
	else if(action == GLFW_PRESS)
		std::cout << "KEY --> " << key << "\n";
}

void Application::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	
	
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	
    //~ if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        //~ std::cout << "Right Mouse Btn Pressed\n";
    //~ else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        //~ std::cout << "Left Mouse Btn Pressed\n";        
}

Application::Application()
{
	std::cout <<  "Creating New Application\n";
	
	glfwSetWindowUserPointer(window.win, this);
	
	glfwSetKeyCallback(window.win, key_callback);
	glfwSetCursorPosCallback(window.win, cursor_pos_callback);
	glfwSetMouseButtonCallback(window.win, mouse_button_callback);
	//~ glfwSetScrollCallback(app.window.win, scroll_callback);
}


Application::~Application()
{	
	std::cout <<  "Deleting Application\n";
}

void Application::printObjectsInfos(){
	for (int i = 0; i < objects.size(); i++)
	{
		Object* curObj = objects[i];
		std::cout << "Object : " << i << "\n";
		std::cout << "\t N vertices : " << curObj->mesh.vertices.size() <<"\n";
		std::cout << "\t position : " << curObj->position.x <<", "<< curObj->position.y <<", "<< curObj->position.z <<"\n";
		std::cout << "\t scale : " << curObj->scale.x <<", "<< curObj->scale.y <<", "<< curObj->scale.z <<"\n";
		
		
		
		
	}
	
}



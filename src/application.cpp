#include "application.h"
#include <iostream>


Application::Application()
{
	std::cout <<  "Creating New Application\n";
	

}


Application::~Application()
{
	
	std::cout <<  "Deleting Application\n";
}

void Application::printObjectsInfos(){
	for (int i = 0; i < objects.size(); i++)
	{
		Object& curObj = objects[i];
		std::cout << "Object : " << i << "\n";
		std::cout << "\t N vertices : " << curObj.mesh.vertices.size() <<"\n";
		std::cout << "\t position : " << curObj.position->x <<", "<< curObj.position->y <<", "<< curObj.position->z <<"\n";
		std::cout << "\t scale : " << curObj.scale->x <<", "<< curObj.scale->y <<", "<< curObj.scale->z <<"\n";
		
		
		
		
	}
	
}



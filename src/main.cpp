#include <iostream>
#include "application.h"
#include "pch.h"

#include "generators/gridmesh.h"
#include "generators/spheremesh.h"

Object* obj1 = new Object();
//~ Object obj2;

int nCols = 4;
Application app ;



Mesh loadNewObject(){
	char file[1024];
	FILE *f = popen("zenity --file-selection --title='choose a 3d file' ", "r");
	fgets(file, 1024, f);	
	std::cout << "----- "<<file << "\n";	
	
	ObjLoader loader;
	Mesh mesh;
	std::string str(file);
	
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	//~ mesh = loader.assimp_load("../src/res/obj/sphere_normals.obj");
	mesh = loader.assimp_load(str);
	
	return mesh;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//~ std::cout << "X offset :" << xoffset << "| ";
	//~ std::cout << " Y offset :" << yoffset << "\n";
	
	obj1->scale *= 1.0 + (float)yoffset*0.1;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//~ std::cout << "key ->" << key << ", scancode --> "<< scancode << "\n";
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
    
        nCols +=3;
        //~ Mesh emptyMesh;
        obj1->mesh.clearAll();
        
        SphereMesh sphere{};
        sphere.generate(obj1->mesh,20,nCols);
        
        
        
        std::cout << nCols << "\n";
        obj1->buildVbo();
	}else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		if( nCols > 4){
			nCols -=3;
			obj1->mesh.clearAll();
			SphereMesh sphere{};
			sphere.generate(obj1->mesh,20,nCols);
			std::cout << nCols << "\n";
			obj1->buildVbo();

		}
	}else if (key == 79 /* letter o*/ && action == GLFW_PRESS){
		
		nCols -=3;
		obj1->mesh.clearAll();
		obj1->mesh = loadNewObject();
		std::cout << nCols << "\n";
		obj1->buildVbo();
	}
}

int main(){
	
	

	std::cout << "angine PROJECT\n";
	
    //obj1->mesh = loadNewObject();
	

	

	
	
        SphereMesh* sphere = new SphereMesh();
        sphere->generate(obj1->mesh,20,nCols);        
        delete sphere;
	
	

	obj1->color.x = 0.9;
	obj1->color.y = 0.8;
	obj1->color.z = 0.5;
	obj1->color.w = 1.0;
	
	obj1->position.x = 1.2f;
	
	obj1->shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	obj1->shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");	
	
	obj1->buildVbo();
	obj1->buildTexture();
	obj1->shader.createShader();

	
	app.objects.push_back(obj1);	
	app.window.objects = app.objects;
//~ 
	//~ Mesh testMesh;
	//~ testMesh = loader.load2("../src/res/obj/sphere_normals.obj");

	glfwSetKeyCallback(app.window.win, key_callback);
	glfwSetScrollCallback(app.window.win, scroll_callback);
	
	while(!app.window.shouldClose()){
		
		app.window.refresh();
		
		obj1->rotation.x = glfwGetTime()*0.2;
		obj1->rotation.y = glfwGetTime()*0.13;
		obj1->rotation.z = glfwGetTime()*0.11;
		
	
	}
	
	return 0;
}

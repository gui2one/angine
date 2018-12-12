#include <iostream>
#include "application.h"
#include "pch.h"

#include "generators/gridmesh.h"
#include "generators/spheremesh.h"

Object obj1;
Object obj2;

int nCols = 4;
Application app ;
SphereMesh sphere;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
    
        nCols +=3;
        //~ Mesh emptyMesh;
        obj1.mesh.clearAll();
        
        sphere.generate(obj1.mesh,20,nCols);
        
        std::cout << nCols << "\n";
        obj1.buildVbo();
        
        app.objects.clear();
        
        app.objects.push_back(obj1);
        app.window.objects.clear();
        app.window.objects.push_back(obj1);
        
        
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		if( nCols > 4){
			nCols -=3;
			obj1.mesh.clearAll();
			
			sphere.generate(obj1.mesh,20,nCols);
			std::cout << nCols << "\n";
			obj1.buildVbo();
			
			app.objects.clear();
			
			app.objects.push_back(obj1);
			app.window.objects.clear();
			app.window.objects.push_back(obj1);
		}
	}
}

int main(){
	
	std::cout << "angine PROJECT\n";


	ObjLoader loader;
	//~  // add object
	
	//~ obj1.mesh = loader.assimp_load("../src/res/obj/sphere_normals_uvs.obj");
	
	sphere.generate(obj1.mesh,20,100);
	//~ obj1.printMeshData();
	

	obj1.color->x = 0.9;
	obj1.color->y = 0.8;
	obj1.color->z = 0.5;
	obj1.color->w = 1.0;
	
	obj1.position->x = 1.2f;
	
	obj1.shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	obj1.shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");	
	
	obj1.buildVbo();
	obj1.buildTexture();
	obj1.shader.createShader();

	
	app.objects.push_back(obj1);
	
	
	
	
	//~ obj2.mesh = loader.assimp_load("../src/res/obj/plane_normals_uvs.obj");
	//~ 
	//~ GridMesh grid;
	//~ obj2.mesh = grid.generate(5,5,5.0);
	//~ obj2.printMeshData();
	//~ obj2.scale->x = 1.0f;
	//~ obj2.scale->y = 1.0f;
	//~ obj2.scale->z = 1.0f;
	//~ 
	//~ obj2.position->x = -1.5f;
	//~ obj2.shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	//~ obj2.shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");	
	//~ 
	//~ obj2.buildVbo();
	//~ obj2.texture->load("../src/res/textures/Basketball.png");
	//~ 
	//~ obj2.shader.createShader();	
	//~ 
	//~ app.objects.push_back(obj2);
	
	
	//~ app.printObjectsInfos();
	
	app.window.objects = app.objects;
//~ 
	//~ Mesh testMesh;
	//~ testMesh = loader.load2("../src/res/obj/sphere_normals.obj");

	glfwSetKeyCallback(app.window.win, key_callback);
	while(!app.window.shouldClose()){
		
		app.window.refresh();
		
		obj1.rotation->x = glfwGetTime()*0.2;
		obj1.rotation->y = glfwGetTime()*0.13;
		obj1.rotation->z = glfwGetTime()*0.11;
		
	
	}
	
	return 0;
}

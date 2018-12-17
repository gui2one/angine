#include <iostream>
#include "application.h"
#include "pch.h"

#include "generators/gridmesh.h"
#include "generators/spheremesh.h"

Object* obj1 = new Object();
Object* obj2 = new Object();

int nRows = 8;
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
	std::cout << "key ->" << key << ", scancode --> "<< scancode << "\n";
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
    
        nCols +=3;
        obj1->mesh.clearAll();
        
        SphereMesh sphere{};
        //~ sphere.generate(obj1->mesh,nCols*1.5,nCols);
        obj1->mesh = sphere.generate2(nRows,nCols);
        
        
        
        std::cout << nCols << "\n";
        obj1->buildVbo();
	}else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
		if( nCols > 4){
			nCols -=3;
			obj1->mesh.clearAll();
			SphereMesh sphere{};
			//~ sphere.generate(obj1->mesh,nCols*1.5,nCols);
			obj1->mesh = sphere.generate2(nRows,nCols);
			std::cout << nCols << "\n";
			obj1->buildVbo();

		}
	}else if (key == 79 /* letter o*/ && action == GLFW_PRESS){
		
		
		obj1->mesh.clearAll();
		obj1->mesh = loadNewObject();
		
		std::cout << nCols << "\n";
		//~ obj1->buildTexture();
		//~ obj1->texture.load("../src/res/textures/Basketball.png");	
		//~ obj1->texture.bind();	
		obj1->buildVbo();

	}
}

int main(){
	
	

	std::cout << "angine PROJECT\n";

	
	//~ ObjLoader loader2;
	//~ obj1->mesh = loader2.assimp_load("../src/res/obj/sphere_normals_uvs.obj");


	SphereMesh sphere{};	
	obj1->mesh = sphere.generate2(nRows,nCols);
	//~ obj1->printMeshData();

	
	obj1->position.x = 1.2f;
	
	obj1->shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	obj1->shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");	
	obj1->buildTexture();
	obj1->shader.createShader();
	
	obj1->buildVbo();

	//~ obj1->setRenderMode(GL_POINTS);

	
	app.objects.push_back(obj1);	

	GridMesh grid{};
	
	
	obj2->mesh = grid.generate(2,2,3,3);
	
	//~ obj2->buildTexture();
	obj2->texture.load("../src/res/fonts/samplefont.gif");
	obj2->texture.bind();
	obj2->shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	obj2->shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");		
	obj2->shader.createShader();
	
	obj2->buildVbo();
	
	obj2->position.x = -2.0;
	app.objects.push_back(obj2);
	
	
	app.window.objects = app.objects;


	glfwSetKeyCallback(app.window.win, key_callback);
	glfwSetScrollCallback(app.window.win, scroll_callback);
	
	while(!app.window.shouldClose()){
		
		app.window.refresh();
		
		obj1->rotation.x = glfwGetTime()*0.2;
		obj1->rotation.y = glfwGetTime()*0.13;
		obj1->rotation.z = glfwGetTime()*0.11;
		
		obj2->rotation.x = glfwGetTime()*0.15;
		obj2->rotation.y = glfwGetTime()*0.19;
		obj2->rotation.z = glfwGetTime()*0.21;		
		
	
	}
	
	return 0;
}

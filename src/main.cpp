#include <iostream>
#include "application.h"
#include <unistd.h>


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
	std::string str(file);	
	//// remove end of line 
		str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	
	
	Mesh mesh;
	mesh = loader.assimp_load(str);
	
	return mesh;
}


int main()
{

	std::cout << "angine PROJECT\n";
	
	//~ ObjLoader loader2;
	//~ obj1->mesh = loader2.assimp_load("../src/res/obj/sphere_normals_uvs.obj");


	SphereMesh sphere{};	
	//~ obj1->mesh = sphere.generate(nRows,nCols);
	//~ obj1->printMeshData();
	

	obj1->init();
	
	obj1->position.x = 1.2f;
	
	//~ obj1->shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	//~ obj1->shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");	
	//~ obj1->buildTexture();
	//~ obj1->shader.createShader();
	//~ 
	//~ obj1->buildVbo();
	//~ obj1->setRenderMode(GL_POINTS);	
	app.objects.push_back(obj1);	
	
	
	//~ GridMesh grid{};
	//~ obj2->mesh = grid.generate(2,2,3,3);
	
	//~ obj2->buildTexture();
	//~ obj2->texture.load("../src/res/fonts/samplefont.gif");
	//~ obj2->texture.bind();
	//~ obj2->shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	//~ obj2->shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");		
	//~ obj2->shader.createShader();
	
	//~ obj2->buildVbo();
	
	obj2->init();
	
	BoundingBox bbox = obj2->getBoundingBox();
	printf("bbox data : \n\tposition x: %f, y: %f, z: %f\n", bbox.position.x, bbox.position.y, bbox.position.z);	
	obj2->position.x = -2.0;
	//~ app.objects.push_back(obj2);	
	app.addObject(obj2);
	//~ app.window.objects = app.objects;



	
	
	
	
	while(!app.window.shouldClose())
	{		
		//~ obj1->rotation.x = glfwGetTime()*0.2;
		//~ obj1->rotation.y = glfwGetTime()*0.13;
		//~ obj1->rotation.z = glfwGetTime()*0.11;
		
		//~ obj2->rotation.x = glfwGetTime()*1.5;
		//~ obj2->rotation.y = glfwGetTime()*1.9;
		//~ obj2->rotation.z = glfwGetTime()*2.1;	
				
		app.window.refresh();
		
	
		//~ usleep(1000000/60);
	
	}
	
	return 0;
}

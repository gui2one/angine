#ifndef OBJLOADER_H
#define OBJLOADER_H
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "../mesh.h"


#include "../include/assimp/cimport.h"
#include "../include/assimp/scene.h"
#include "../include/assimp/postprocess.h"

class ObjLoader{
	public:
		ObjLoader();
		~ObjLoader();
		Mesh assimp_load(std::string file_path);
		

};

#endif

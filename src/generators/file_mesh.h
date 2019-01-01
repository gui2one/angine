#ifndef FILE_MESH_H
#define FILE_MESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
#include "../utils/objLoader.h"
class FileMesh : public MeshGenerator
{
	public:
		FileMesh();
		virtual ~FileMesh();
		
		Mesh generate() override;
		
		
		
		std::string file_path = "/home/pi/projects/angine/src/res/obj/monkey.obj";
	private:
		/* add your private declarations */
};

#endif /* FILE_MESH_H */ 

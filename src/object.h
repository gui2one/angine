#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>

#include "pch.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"


class Object{
	public :
		Object();
		~Object();

		void buildVbo();
		
		unsigned int m_vbo;
		unsigned int m_normals_vbo;
		
		void buildTexture();
		
		void draw();
		
		void drawNormals();
		bool bDisplayNormals = false;
		
		void printMeshData();
		Mesh mesh;
		std::vector<float> normals_data;
		Shader shader;
		GLint texture_id;
		Texture* texture;
		//~ std::string texture_path = "";
		
		glm::vec3* position;
		glm::vec3* scale;
		glm::vec3* rotation;
		glm::vec4* color;
		
		
		
	
};

#endif

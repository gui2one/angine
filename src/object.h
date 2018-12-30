#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>

#include "pch.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include "generators/gridmesh.h"
#include "generators/spheremesh.h"
#include "generators/box_mesh.h"
#include "generators/file_mesh.h"
#include "generators/geosphere_mesh.h"

#include <typeinfo>


struct uniform{
	std::string uniform_name;
	GLsizei length;
	GLint size;
	GLenum type;	
};


struct BoundingBox{
	glm::vec3 position;
	glm::vec3 size;
};

class Object{
	public :
		Object();
		~Object();

		std::vector<uniform> getShaderUniforms();
		void buildVbo();
		
		unsigned int m_vbo, m_ibo, m_normals_vbo, m_bbox_vbo;
		
		void init();
		void buildTexture();
		
		inline void setRenderMode(GLuint mode){ renderMode = mode;}
		inline GLuint getRenderMode(){ return renderMode;}
		void draw(GLuint mode=GL_TRIANGLES);
		
		void drawBoundingBox();
		bool bDisplayBoundingBox = false;
		void drawNormals();
		bool bDisplayNormals = false;
		
		void drawPoints();
		bool bDisplayPoints = false;
		bool bDisplayWireframe = false;
		
		void computeBoundingBox();
		BoundingBox getBoundingBox();
		
		
		bool has_generator = false;
		int generator_type = -1;
		MeshGenerator* mesh_generator;
		
		template<typename T> 
		inline void setGenerator(){
			
			
			//~ std::cout << typeid(T).name() << "\n";
			
			T* generator = new T();
			
			mesh.vertices.clear();
			mesh.indices.clear();
			mesh = generator->generate();
			buildVbo();
			computeBoundingBox();
			
			mesh_generator = generator;
			has_generator = true;
			
		}
		
		
		void printMeshData();
		Mesh mesh;
		std::vector<float> vertex_data;
		std::vector<float> normals_data;
		Shader shader, lineShader;
		GLint texture_id;
		Texture texture;
		//~ std::string texture_path = "";
		
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec4 color;
		
		char name[100] = {'a','b','c'};
		BoundingBox boundingBox;
		private:
			GLuint renderMode = GL_TRIANGLES;
			
};

#endif

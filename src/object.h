#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>

#include "pch.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

struct uniform{
	std::string uniform_name;
	GLsizei length;
	GLint size;
	GLenum type;	
};

class Object{
	public :
		Object();
		~Object();

		std::vector<uniform> getShaderUniforms();
		void buildVbo();
		
		unsigned int m_vbo, m_ibo, m_normals_vbo;
		
		
		void buildTexture();
		
		inline void setRenderMode(GLuint mode){ renderMode = mode;}
		inline GLuint getRenderMode(){ return renderMode;}
		void draw(GLuint mode=GL_TRIANGLES);
		
		void drawNormals();
		bool bDisplayNormals = false;
		
		void printMeshData();
		Mesh mesh;
		std::vector<float> vertex_data;
		std::vector<float> normals_data;
		Shader shader;
		GLint texture_id;
		Texture texture;
		//~ std::string texture_path = "";
		
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::vec4 color;
		
		private:
			GLuint renderMode = GL_TRIANGLES;
};

#endif

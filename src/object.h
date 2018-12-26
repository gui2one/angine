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
		
		void computeBoundingBox();
		BoundingBox getBoundingBox();
		
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
		
		private:
			GLuint renderMode = GL_TRIANGLES;
			BoundingBox boundingBox;
};

#endif

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <iterator>

#include "pch.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"


#include "generators/gridmesh.h"
#include "generators/spheremesh.h"
#include "generators/box_mesh.h"
#include "generators/file_mesh.h"
#include "generators/geosphere_mesh.h"
#include "generators/cylinder_mesh.h"

#include "mesh_filters/transform_mesh_filter.h"
#include "mesh_filters/inflate_mesh_filter.h"
#include "mesh_filters/twist_mesh_filter.h"
#include "mesh_filters/compute_normals_mesh_filter.h"
#include "mesh_filters/spherify_mesh_filter.h"
#include "mesh_filters/duplicate_mesh_filter.h"
#include "mesh_filters/from_polar_mesh_filter.h"
#include "mesh_filters/mirror_mesh_filter.h"

#include "entity3d.h"

#include "vendor/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;
////

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

class Object : public Entity3D
{
	public :
		Object();
		Object(const Object& other);
		~Object();

		std::vector<uniform> getShaderUniforms();
		void buildVbo();

		
		void init();
		void initShader();		
		void buildTexture();		
		
		
		
		
		inline void setRenderMode(GLuint mode){ renderMode = mode;}
		inline GLuint getRenderMode(){ return renderMode;}		
		
		unsigned int m_vbo, m_ibo, m_normals_vbo, m_bbox_vbo;

		bool bDisplayPolygons = true;
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
		BoundingBox computeAABB();
		
		
		bool has_generator = false;
		int generator_type = -1;
		MeshGenerator* mesh_generator;
		
		
		template<typename T> 
		inline void setGenerator(){
			
			T* generator = new T();
			
			mesh.vertices.clear();
			mesh.indices.clear();
			mesh = generator->generate();
			
			computeBoundingBox();
			
			mesh_generator = generator;
			has_generator = true;			
		}
		
	
		std::vector<MeshFilter*> meshFilters;
		bool hasFilters = false;
		void moveFilter(int _origin, int _dest);
		template<typename T>
		inline void setMeshFilter(){
			T* filter = new T();			
			mesh = filter->applyFilter(mesh);			
			meshFilters.push_back(filter);
		}
		
		
		
		void updateMesh();
		
		void printMeshData();
		
		Mesh mesh;
		std::vector<float> vertex_data;
		std::vector<float> normals_data;
		
		Shader shader;
		GLint texture_id;
		Texture texture;
		

		
		
		
		BoundingBox boundingBox;
		
		json toJSON();
		
		
		private:
			GLuint renderMode = GL_TRIANGLES;
			//~ Entity3D* parent = nullptr;
			
};

#endif

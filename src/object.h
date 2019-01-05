#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>

#include "pch.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

#include "param_layout.h"


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
		void initShader();
		
		void buildTexture();
		
		inline void setRenderMode(GLuint mode){ renderMode = mode;}
		inline GLuint getRenderMode(){ return renderMode;}
		
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
			//~ buildVbo();
			computeBoundingBox();
			
			mesh_generator = generator;
			has_generator = true;			
		}
		
		
		
		std::vector<MeshFilter*> meshFilters;
		bool hasFilters = false;
		
		template<typename T>
		inline void setMeshFilter(){
			T* filter = new T();
			
			mesh = filter->applyFilter(mesh);
			//~ buildVbo();
			meshFilters.push_back(filter);
		}
		
		inline void updateMesh()
		{
			if(has_generator){
				
				if( mesh_generator->need_update)
				{
					printf("---> Generate Mesh\n");
					mesh = mesh_generator->generate();
					mesh_generator->need_update = false;
					
					// force filters need_update
					for (int i = 0; i < meshFilters.size(); i++)
					{
						meshFilters[i]->need_update = true;
					}
					
					buildVbo();
				}
			}

			if(hasFilters)
			{		
				//// check first if one of the filters need update
				bool some_need = false;
				for (int i = 0; i < meshFilters.size(); i++)
				{
					if(meshFilters[i]->need_update){
						some_need = true;
					}
				}
						
				for (int i = 0; i < meshFilters.size(); i++)
				{
					if(i == 0 && some_need){
						mesh = mesh_generator->mesh_cache;	
						printf("---> Loading Generator Cache\n");
						some_need = false;
					}					
					
					if(meshFilters[i]->is_active)					
					{
						if(meshFilters[i]->need_update)
						{
							printf("applying filter\n");
							mesh = meshFilters[i]->applyFilter(mesh);
							meshFilters[i]->need_update = false;
						}else{
							mesh = meshFilters[i]->mesh_cache;
							meshFilters[i]->need_update = false;
						}
						
						
					}
					
					meshFilters[i]->need_update = false;
				}
				
				//~ if(some_need == false){
					//~ mesh = mesh_generator->mesh_cache;
				//~ }
				buildVbo();
			}
			
			
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
		
		char name[100] = {'n','e','w','_','o','b','j','e','c','t'};
		BoundingBox boundingBox;
		
		
		
		private:
			GLuint renderMode = GL_TRIANGLES;
			
};

#endif

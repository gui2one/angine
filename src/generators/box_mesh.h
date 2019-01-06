#ifndef BOX_MESH_H
#define BOX_MESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"

struct TRI_PLANE{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;	
};
class BoxMesh : public MeshGenerator
{
	public:
		BoxMesh();
		virtual ~BoxMesh();
		
		inline Mesh generate() override{
			Mesh mesh;
			mesh = generateBox( 
								p_width->getValue(), 
								p_height->getValue(), 
								p_length->getValue(),								
								p_segs_width->getValue(),
								p_segs_height->getValue(),
								p_segs_length->getValue()
								
								);
			
			mesh_cache = mesh;
			return mesh;
		};
		Mesh generateBox(float width = 1.0, float height = 1.0, float length = 1.0, int segs_width = 2, int segs_height = 2, int segs_length = 2);
		
		
		ParamFloat* p_width;
		ParamFloat* p_height;
		ParamFloat* p_length;
		
		ParamInt* p_segs_width;
		ParamInt* p_segs_height;
		ParamInt* p_segs_length;
	private:
		/* add your private declarations */
};

#endif /* BOX_MESH_H */ 

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
								paramsFloat[0].value , 
								paramsFloat[1].value, 
								paramsFloat[2].value,
								
								paramsInt[0].value,
								paramsInt[1].value,
								paramsInt[2].value
								
								);
			
			return mesh;
		};
		Mesh generateBox(float width = 1.0, float height = 1.0, float length = 1.0, int segs_width = 2, int segs_height = 2, int segs_length = 2);
		
	private:
		/* add your private declarations */
};

#endif /* BOX_MESH_H */ 

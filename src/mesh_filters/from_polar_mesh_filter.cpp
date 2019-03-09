#include "from_polar_mesh_filter.h"


FromPolarMeshFilter::FromPolarMeshFilter()
	: MeshFilter()
{
	p_menu = new ParamMenu(
			"transform order", 
			std::vector<std::string>{
					"Rot/Scale/Trans","Scale/Rotate/Trans","Scale/Trans/Rotate"});
	param_layout.push(p_menu);

	p_translate = new ParamVec3("translate",glm::vec3( 0.0f, 0.0f, 0.0f),"t_");
	param_layout.push(p_translate);
	
	p_rotate = new ParamVec3("rotate",glm::vec3( 0.0f, 0.0f, 0.0f),"r_");
	param_layout.push(p_rotate);	
	
	p_scale = new ParamVec3("scale",glm::vec3( 1.0f, 1.0f, 1.0f),"s_");
	param_layout.push(p_scale);	
}

FromPolarMeshFilter::FromPolarMeshFilter(const FromPolarMeshFilter& other):MeshFilter(other){
	p_menu = new ParamMenu(*other.p_menu);
	param_layout.push(p_menu);

	p_translate = new ParamVec3(*other.p_translate);
	param_layout.push(p_translate);
	
	p_rotate = new ParamVec3(*other.p_rotate);
	param_layout.push(p_rotate);	
	
	p_scale = new ParamVec3(*other.p_scale);
	param_layout.push(p_scale);		
}

static glm::vec3 fromPolar(float u, float v, float radius){
	glm::vec3 temp;
	
	temp.x = radius * sin(u) * cos(v);
	temp.y = radius * sin(u) * sin(v);
	temp.z = radius * cos(u);
	
	return temp;
}

Mesh FromPolarMeshFilter::applyFilter(Mesh & source_mesh)
{
	Mesh mesh;
	mesh = source_mesh;
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		mesh.vertices[i].position = fromPolar( mesh.vertices[i].position.x, mesh.vertices[i].position.y, 1.0);
	}	
	
	mesh_cache = mesh;
	return mesh;
};



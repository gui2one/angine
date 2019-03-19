#include "mesh_filter.h"

MeshFilter::MeshFilter()
{
	
}

MeshFilter::MeshFilter(const MeshFilter& other){
	//~ printf("MeshFilter COPY CONSTRUCTOR\n");
	
	setName(other.name);
}
void MeshFilter::setName(std::string str_name)
{
	//~ for (int i = 0; i < 200; i++)
	//~ {
		//~ name[i] = ' ';
	//~ }
	
	
	for (int i = 0; i < str_name.size(); i++)
	{
		name[i] = str_name[i];
	}
	
}
Mesh MeshFilter::applyFilter(Mesh & mesh){
	
}

json MeshFilter::toJSON()
{
	json j;
	
	
	j["name"] = name;
	j["params"] = param_layout.toJSON();
	j["type"] = getType();
	//~ printf("params num is --> %d\n", param_layout.getSize());
	
	return j;
}

void MeshFilter::fromJSON(json& _json)
{
	
}

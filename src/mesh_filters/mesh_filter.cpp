#include "mesh_filter.h"

MeshFilter::MeshFilter()
{
	
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

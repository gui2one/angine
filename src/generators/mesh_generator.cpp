#include "mesh_generator.h"


MeshGenerator::MeshGenerator()
{
	
}

MeshGenerator::MeshGenerator(const MeshGenerator& other){
	//~ printf("MeshGenerator COPY CONSTRUCTOR\n");
	mesh_cache = other.mesh_cache;
		
	//~ param_layout = other.param_layout;	
}

json MeshGenerator::toJSON()
{
	json j;
	//~ j["name"] = name;
	
	j["type"] = type;
	
	j["params"] = param_layout.toJSON();
	//~ 
	//~ std::vector<json> all_params;
	//~ for (int i = 0; i < param_layout.getSize(); i++)
	//~ {
		//~ BaseParam * p_parm = param_layout.getParam(i);
		//~ ParamFloat * p_float = nullptr;
		//~ ParamInt * p_int = nullptr;
//~ 
		//~ if(p_float = dynamic_cast<ParamFloat*>(p_parm)){
//~ 
			//~ all_params.push_back(p_float->toJSON());
		//~ }else if(p_int = dynamic_cast<ParamInt*>(p_parm)){
			//~ 
			//~ all_params.push_back(p_int->toJSON());
		//~ }
//~ 
		
	//~ }
	//~ 
	//~ j["params"] = all_params;
	
	return j;
	
	
}

void MeshGenerator::fromJSON(json _j)
{
	printf("MeshGenerator fromJSON function fired\n");
	//~ return new CylinderMeshGenerator();
}
MeshGenerator::~MeshGenerator(){
	for (size_t i = 0; i < param_layout.getSize(); i++)
	{
		delete param_layout.getParam(i);
	}
	
	//~ printf("Just delete paramLayout content\n");			
}



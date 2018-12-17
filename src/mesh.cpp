#include "mesh.h"

Mesh::Mesh(){
	//~ std::cout << "Mesh instance Created\n";
}


void Mesh::clearAll(){
	vertices.clear();
	indices.clear();
}

//~ Mesh::~Mesh(){
	//~ vertices.clear();		
	//~ indices.clear();
	//~ std::cout << "--- Mesh instance Deleted ----\n";
//~ }

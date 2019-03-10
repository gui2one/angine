#ifndef GIZMO_H
#define GIZMO_H


#include "../pch.h"
#include "../mesh.h"
#include "../entity3d.h"
#include "../shader.h"

//~ #include "param_layout.h"
class Gizmo
{
	public:
		Gizmo();
		// copy constructor
		Gizmo(const Gizmo& other);		
		virtual ~Gizmo();
	
		glm::mat4 transforms = glm::mat4(1.0);		
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		

		void buildVbo();
		Mesh mesh;
		//~ Shader shader;
		std::vector<float> vertex_data;
		std::vector<float> normals_data;
		unsigned int m_ibo, m_vbo;
		glm::vec3 getWorldPosition();

		void draw(Shader & _shader, Entity3D& target_object);
		
		inline void setID(int _id){	id = _id;}			
		inline int getID(){	return id;}

		
		void setName(std::string str_name);
		char name[100] = {'n','e','w','_','o','b','j','e','c','t'};
		glm::vec4 color;
		
		

	private:

		int id;
		/* add your private declarations */
};

#endif /* GIZMO_H */ 

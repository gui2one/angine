#ifndef GIZMO_H
#define GIZMO_H


#include "../pch.h"
#include "handle.h"
#include "../mesh.h"
#include "../entity3d.h"
#include "../shader.h"
#include "../camera.h"

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
		

		virtual void buildVbo(){}
		virtual void draw(Shader & _shader, Camera & camera){}
		
		Mesh mesh;
		//~ Shader shader;
		std::vector<float> vertex_data;
		std::vector<float> normals_data;
		unsigned int m_ibo, m_vbo;
		glm::vec3 getWorldPosition();

		
		
		inline void setID(int _id){	id = _id;}			
		inline int getID(){	return id;}

		
		void setName(std::string str_name);
		char name[100] = {'n','e','w','_','o','b','j','e','c','t'};
		glm::vec4 color;
		
		
		Entity3D* target_object = nullptr;
		
		
		std::vector<Handle *> handles;
		int active_handle_id = -1;

	private:

		int id;
		/* add your private declarations */
};

class TranslationGizmo : public Gizmo
{
	public :
		TranslationGizmo();
		void buildVbo() override;
		void draw(Shader & _shader, Camera & camera) override;

};
#endif /* GIZMO_H */ 

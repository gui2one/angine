#ifndef HANDLE_H
#define HANDLE_H

#include "../pch.h"
#include "../param.h"
#include "../mesh.h"
#include "../shader.h"
#include "../camera.h"


class Handle
{
	public:
		Handle();
		
		virtual void draw(Shader & _shader, Camera & camera, glm::mat4 model_matrix)
		{
			//~ printf("Base Handle Draw \n");
		}		
		virtual void buildVbo()
		{
			printf("Base Handle BuildVbo \n");
		}	
		
		inline void setName(std::string _name) 
		{
			name = _name;
		}
		
		inline std::string getName() const
		{
			return name;
		}
		
		Mesh mesh;
		unsigned int m_vbo, m_ibo;
		std::vector<float> vertex_data;
			
	private:
		BaseParam* target_param;
		std::string name;
		/* add your private declarations */
};


class TranslateHandle : public Handle
{
	public:
		TranslateHandle();
		virtual void buildVbo() override;
		virtual void draw(Shader & _shader, Camera & camera, glm::mat4 model_matrix) override;
		
		int axis = 0;
				
		
};
#endif /* HANDLE_H */ 

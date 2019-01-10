#ifndef WINDOW_H
#define WINDOW_H

#include "pch.h"

#include "object.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
//~ #include "generators/mesh_generator.h"
//~ #include "mesh_filters/mesh_filter.h"

//~ #include <unistd.h>

//~ #define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Window
{
	public:
		Window();
		virtual ~Window();
		void refresh();
		
		GLFWwindow * win;
		Camera camera;
		
		
		//~ unsigned int sel_gizmo_vbo;
		//~ unsigned int sel_gizmo_ibo;
		//~ void selGizmoInit();
		//~ void drawSelGizmo();
		
		
		
		int width, height;
		bool shouldClose();
		

		
		std::vector<Object*> objects;
		
		void addObject(Object* obj);
		void removeObject(Object* obj);
		int findObjectIndexByID(int id);
		void buildObjectList();
		
				
		void renderObjects();
		

		Shader pointShader;
		
		//~ unsigned int shader_id;
		
		glm::mat4 ModelViewProjectionMatrix;
		GLuint PROJECTIONLoc, MODEL_LOC;
		
		
		int explorerDialog();
		std::string current_explorer_path = "/";
		
		void objectPropertiesDialog();
		void objectListDialog();
		int cur_object_selected = 0;
		int cur_mesh_filter_selected = 0;
		
		int cur_unique_id = 0;
	private:
		
		
		/* add your private declarations */
};

#endif /* WINDOW_H */ 

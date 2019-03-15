#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <type_traits>
#include "pch.h"

#include "gizmos/gizmo.h"

#include "object.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

#include "animation/time_line.h"
//~ #include "generators/mesh_generator.h"
//~ #include "mesh_filters/mesh_filter.h"

//~ #include <unistd.h>

//~ #define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "vendor/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;
////

class Window
{
	public:
		Window();
		virtual ~Window();
		void refresh();
		
		GLFWwindow * win;
		static void char_mods_callback(GLFWwindow* window, unsigned int key, int action);
		static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);	
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		
		
		Entity3D* mouseClickObject();
		bool mouseClickGizmo();
		void initWorldGrid();
		void drawWorldGrid();
		unsigned int world_grid_vbo, world_grid_ibo;
		
		TimeLine time_line;
		bool right_mouse_button_down = false;	
		Camera camera;
		
		void setCamPosFromPolar(float u, float v, float _radius);
		double camera_orbit_radius = 5.0;
		double camera_u_pos = PI;
		double camera_v_pos = (PI/2.0) - 0.3;
		double mouse_pos_x, mouse_pos_y;
		double mouse_delta_x = 0.0;
		double mouse_delta_y = 0.0;
		double mouse_old_x, mouse_old_y;

		
		void buildParamUiKeyframePopupBegin(BaseParam * param);

		void buildParamUiKeyframePopupEnd(BaseParam * param,std::function<void()> callback,  int vec3_ID=-1);		
		void drawKeyframes(BaseParam* _param, int selected_key_id);
		int width, height;
		bool shouldClose();
		

		
		std::vector<Entity3D*> objects;
		
		void addObject(Entity3D* obj);
		void removeObject(Entity3D* obj);
		Entity3D duplicateObject(Entity3D * obj);
		int findObjectIndexByID(int id);
		void buildObjectList();
		
		
		std::vector<Gizmo *> gizmos;
				
		void renderObjects();
		

		Shader default_shader,point_shader, wireframe_shader;
		
		//~ unsigned int shader_id;
		
		glm::mat4 ModelViewProjectionMatrix;
		GLuint PROJECTIONLoc, MODEL_LOC;
		
		
		int explorerDialog();
		std::string current_explorer_path = "/";
		
		void objectPropertiesDialog();
		void objectListDialog();
		
		void timeLineDialog();
		void evalKeyframes();
		
		int cur_object_selected = -1;
		int cur_mesh_filter_selected = 0;
		
		int cur_unique_id = 0;
		
		//~ template<typename T>
		void buildParamUi(BaseParam * param, std::function<void()> callback = [](){});
		
		void saveToFile();
		void loadFromFile(std::string file_path);
	private:
		
		
		/* add your private declarations */
};

#endif /* WINDOW_H */ 

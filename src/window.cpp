#include "window.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <sstream>
#include <algorithm>

#include <array>

#include "gizmos/gizmo.h"

#include "object.h"
#include "object_dummy.h"
#include "object_light.h"

//~ #include <opensubdiv/far/topologyDescriptor.h>
//~ #include <opensubdiv/far/primvarRefiner.h>


// for explorerDialog
#include <dirent.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/stat.h>

#include "raycaster.h"




static int ray_plane_intersect(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 pointP, glm::vec3 rayDir, glm::vec3& hitP)
{
    glm::vec3 W = planeP - pointP;
    
    float K = glm::dot(W, planeN) / glm::dot( rayDir, planeN);
    
    hitP = pointP + ( K * rayDir);
    return  K>= 0.0 && K <= 1.0;
}

static std::vector<std::string> split(const std::string& str, std::string delimiter = " "){
	
	std::vector<std::string> tokens;
	std::string tmp = str;
	std::string::size_type start = 0;
	while(true)
	{
		std::string::size_type found = tmp.find_first_of(delimiter);
		if( found == std::string::npos )
		{
			//std::cout << " reached end ------------------------------- " << std::endl;	
			tokens.push_back(tmp);
			break;
		}else{
			std::string cut = tmp.substr(0, found);
			tokens.push_back(cut);
			tmp = tmp.substr(found+1, tmp.size() - (found-1));
			//std::cout << tmp << std::endl;
			start = found;
		}
	}
	
	//std::cout << " ------------------------------- " << std::endl;	
	
	return tokens;
}

Window::Window()
{
	
	if(!glfwInit()){
		std::cout<<"Problem with GLFW\n";
		glfwTerminate();		
	}

	std::cout<<"initializing GLFW\n";
	win = glfwCreateWindow(1024,576, "angine", NULL, NULL);


	if(!win){
		std::cout << "Problem with window\n";
		glfwTerminate();		
	}
	
		
	glfwMakeContextCurrent(win);			
	glewInit();
	
	
	glfwSetWindowUserPointer(win, this);
	glfwSetCharModsCallback(win, char_mods_callback);	
	glfwSetCursorPosCallback(win, cursor_pos_callback);	
	glfwSetMouseButtonCallback(win, mouse_button_callback);	
	glfwSetScrollCallback(win, scroll_callback);
	
	
	wireframe_shader.loadVertexShaderSource("../src/res/shaders/wireframe_shader.vert");
	wireframe_shader.loadFragmentShaderSource("../src/res/shaders/wireframe_shader.frag");		
	wireframe_shader.createShader();
		
	point_shader.loadVertexShaderSource("../src/res/shaders/line_shader.vert");
	point_shader.loadFragmentShaderSource("../src/res/shaders/line_shader.frag");		
	point_shader.createShader();	
	
	default_shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	default_shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");		
	default_shader.createShader();	
	
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	
	//~ glEnable(GL_LINE_SMOOTH);
	//~ glEnable(GL_POLYGON_SMOOTH);
	
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	//~ glEnable(GL_CULL_FACE);
	//~ glCullFace(GL_BACK);
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;

	
	const char* glsl_version = "#version 130";

	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	

	initWorldGrid();
	

	setCamPosFromPolar(camera_u_pos, camera_v_pos, camera_orbit_radius);

	// timeline 
	
	time_line.start = 1;
	time_line.end = 100;
	time_line.frame_rate = 25.0;
	

	// gizmo test
	Gizmo * gizmo = new Gizmo();
	gizmo->buildVbo();
	gizmos.push_back(gizmo);

	// default first object
	Object* obj = new Object();
	
	obj->init();
	obj->shader = default_shader;
	obj->setName("wheel_1");
	obj->setGenerator<CylinderMesh>();
	
	
	std::vector<BaseKeyframe *> keyframes;
	
	Keyframe<float> * key_1 = new Keyframe<float>();
	key_1->setFrame((float)time_line.current_frame);
	key_1->setValue(0.2);
	keyframes.push_back(key_1);
	obj->mesh_generator->param_layout.getParam(0)->setKeyframes(keyframes);
	
	ParamFloat * p_float = nullptr;
	if( p_float = dynamic_cast<ParamFloat *>(obj->mesh_generator->param_layout.getParam(0))){
		
		
		float test_val = p_float->getValueAtFrame(time_line.current_frame);
		//~ printf("key value at frame %d --> %.3f\n", time_line.current_frame, test_val);
	}
	
	obj->rotation.y = -45.0;
	obj->applyTransforms();
	obj->generator_type = 6; // really crappy design , do something !!!!
	obj->mesh_generator->need_update = true;
	
	addObject(obj);
	
	
	ObjectDummy * dummy = new ObjectDummy();
	dummy->setName("null_1");
	dummy->init();
	
	dummy->p_pos->param_x->setValue(-1.5);
	dummy->applyTransforms();
	addObject(dummy);

	evalKeyframes();

}

bool Window::mouseClickGizmo()
{
	Raycaster raycaster;
	
	std::vector<Gizmo*> result_gizmos;
	bool hit = raycaster.intersectGizmos(this, camera, gizmos, result_gizmos);
	
	
	
	return hit;
	
}

Entity3D* Window::mouseClickObject()
{
	
	Raycaster raycaster;



	std::vector<Entity3D*> result_objects;
	raycaster.intersectObjects(this, camera, objects, result_objects);
	
	
	
	if( result_objects.size() > 0){
		cur_object_selected = findObjectIndexByID( result_objects[0]->getID());
		return result_objects[0];
	}
	//~ double pos_x, pos_y;
	//~ glfwGetCursorPos(win, &pos_x, &pos_y);

	//~ printf("cur_object_selected is %d\n", cur_object_selected);
	cur_object_selected = -1;
	
	return nullptr;
	
}

void Window::initWorldGrid()
{

	std::vector<float> verts;

	for (int x = 0; x <= 10; x++)
	{
		verts.push_back((float)x-5);
		verts.push_back(-5.0f);
		verts.push_back( 0.0f);
		
		
		verts.push_back((float)x-5);
		verts.push_back( 5.0f);
		verts.push_back( 0.0f);
	}
	
	for (int y = 0; y <= 10; y++)
	{
		verts.push_back(-5.0f);
		verts.push_back((float)y-5);
		verts.push_back( 0.0f);
		
		verts.push_back( 5.0f);
		verts.push_back((float)y-5);
		verts.push_back( 0.0f);
	}	
	
	std::vector<unsigned int> indices;

	for (int i = 0; i <= 21; i++)
	{
		indices.push_back((i*2));
		indices.push_back((i*2)+1);
	}
	

	glDeleteBuffers(1, &world_grid_vbo);
	glGenBuffers(1, &world_grid_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, world_grid_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), verts.data() ,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glDeleteBuffers(1, &world_grid_ibo);
	glGenBuffers(1, &world_grid_ibo);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world_grid_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data() ,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	
	

	
	
}


void Window::drawWorldGrid()
{
	glBindBuffer(GL_ARRAY_BUFFER, world_grid_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); 
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world_grid_ibo);
	
	glDrawElements(GL_LINES, 44, GL_UNSIGNED_INT, nullptr);
	
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if(app->right_mouse_button_down)
	{
		app->mouse_delta_x = app->mouse_old_x - xpos;
		app->mouse_old_x = xpos;
		app->mouse_delta_y = app->mouse_old_y - ypos;
		app->mouse_old_y = ypos;
		
		
		double rot_speed = 0.01;
		
		app->camera_u_pos -= app->mouse_delta_x * rot_speed;
		

			
		app->camera_v_pos += app->mouse_delta_y * rot_speed;
		
		if(app->camera_v_pos < 0.2)
			app->camera_v_pos = 0.2;
		else if(app->camera_v_pos > PI-0.2)
			app->camera_v_pos = PI-0.2;		
		
		app->setCamPosFromPolar(app->camera_u_pos, app->camera_v_pos, app->camera_orbit_radius);
		
	}
	
}


void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Window* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
	ImGuiIO &io = ImGui::GetIO();
	if(io.WantCaptureMouse){		
		//~ printf("ImGui captured mouse\n");
	}else{	
		app->camera_orbit_radius -= yoffset*0.2;
		app->setCamPosFromPolar(app->camera_u_pos, app->camera_v_pos, app->camera_orbit_radius);
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	ImGuiIO &io = ImGui::GetIO();
	if(io.WantCaptureMouse)
	{
		//~ printf("mouse pos %f, %f\n", io.MousePos[0], io.MousePos[1]);		
	}else{	
		
		
		if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS )
		{
			app->right_mouse_button_down = true;
			
			glfwGetCursorPos(window, &app->mouse_pos_x, &app->mouse_pos_y);
			app->mouse_old_x = app->mouse_pos_x;
			app->mouse_old_y = app->mouse_pos_y;			
			
		}else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE ){
			
			app->right_mouse_button_down = false;			
		}
		
		if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
			if( app->cur_object_selected != -1)
			{
				bool is_gizmo = app->mouseClickGizmo();
				
				if(!is_gizmo)
				{
					app->mouseClickObject();
				}			
			}else{
				app->mouseClickObject();
			}
		}
	}
}

void Window::char_mods_callback(GLFWwindow* window, unsigned int key, int mod)
{
	Window* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	if( (char)key == 'l' ) // lower L
		app->buildObjectList();
	if( mod == GLFW_MOD_SHIFT)
	{
		printf("shift + %c\n", std::tolower((char)key));
	}
	
}

void Window::setCamPosFromPolar(float u, float v, float _radius)
{
	camera.position.x = sin(u)* sin(v) * _radius;
	camera.position.y = cos(u)* sin(v) * _radius;
	camera.position.z = cos(v) * _radius;	
}

int Window::explorerDialog()
{
	
	using namespace std;
	DIR* dir;
	struct dirent *sd;
	
	struct stat st_buf;
	int status;
	//~ char currentpath[FILENAME_MAX];
	if((dir = opendir(current_explorer_path.c_str())) == NULL){ /*Opens directory*/
		return errno;
	}
	//~ if(getcwd(currentpath, FILENAME_MAX) == NULL){
		//~ return errno;
	//~ }	
  
	//~ status = stat (current_explorer_path.c_str(), &st_buf);
    //~ if (S_ISREG (st_buf.st_mode)) {
        //~ printf ("%s is a regular file.\n", current_explorer_path.c_str());
    //~ }
    //~ if (S_ISDIR (st_buf.st_mode)) {
        //~ printf ("%s is a directory.\n", current_explorer_path.c_str());
    //~ }	
	



	std::vector<std::string> dir_names;
	std::vector<std::string> file_names;
	
	// collect files and dirs names
	while ((sd= readdir(dir)) != NULL){ /*starts directory stream*/
		if(strcmp(sd -> d_name,".") == 0 || strcmp(sd -> d_name,"..") == 0)
		{
			continue;
		}else{			
			std::string check_path = current_explorer_path;
			check_path += sd->d_name;	
				
			status = stat (check_path.c_str(), &st_buf);
				
			if (!S_ISREG(st_buf.st_mode)) 
			{		
				if(sd->d_name[0] != '.')	
					dir_names.push_back(sd->d_name);
			}else{
				if(sd->d_name[0] != '.')	
					file_names.push_back(sd->d_name);
			}
		}
	}  
	closedir(dir); /* important !!!! */
	
	// sort names vectors
	sort(dir_names.begin(), dir_names.end(), 
	[](std::string& str1, std::string& str2)
	{ 
		std::string lower1 = str1;
		std::string lower2 = str2;
		std::transform(lower1.begin(), lower1.end(), lower1.begin(), ::tolower);
		std::transform(lower2.begin(), lower2.end(), lower2.begin(), ::tolower);
		return lower1 < lower2;
	});
	sort(file_names.begin(), file_names.end(), 
	[](std::string& str1, std::string& str2)
	{ 
		std::string lower1 = str1;
		std::string lower2 = str2;
		std::transform(lower1.begin(), lower1.end(), lower1.begin(), ::tolower);
		std::transform(lower2.begin(), lower2.end(), lower2.begin(), ::tolower);
		return lower1 < lower2;
	});	
	
	
	ImGui::Begin("File explorer");
		
	// make directories a different color
	ImVec4 color = ImColor(1.0f, 1.0f, 0.2f, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	
	if(ImGui::Selectable("..", false))
	{
		
		if( current_explorer_path != "/")
		{
			std::string new_path;
			new_path = "";
			
			std::vector<std::string> splitted = split(current_explorer_path, "/");
			
			for (int i = 0; i < splitted.size()-2; i++)
			{
				new_path += splitted[i];
				new_path += "/";
			}
			
			//~ std::cout << "\n";
				
			current_explorer_path = new_path;		
		}
	}
	for (int i = 0; i < dir_names.size(); i++)
	{
		if(ImGui::Selectable(dir_names[i].c_str(), false)){
			
			current_explorer_path += dir_names[i];	
			current_explorer_path += "/";		
		}	
	}
	// revert back to default color
	ImGui::PopStyleColor();
	
	for (int i = 0; i < file_names.size(); i++)
	{
		if(ImGui::Selectable(file_names[i].c_str(), false)){
			
			std::string selected_file_path = current_explorer_path + file_names[i];

			printf("Selected File is : %s\n", selected_file_path.c_str());

		}	
	}	
	
	
	
	
	ImGui::End();
	
	dir_names.clear();
	file_names.clear();
}

int Window::findObjectIndexByID(int id)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if(objects[i]->getID() == id)
		{
			return i;
		}
	}	
	return -1;	
}

void Window::buildObjectList()
{
	
	std::vector<std::pair<Entity3D*, int > > depths(objects.size());
	
	for (int i = 0; i < objects.size(); i++)
	{
		depths[i].first = objects[i];
		depths[i].second = 0;
		Entity3D * p = objects[i];
		Object * p_obj = nullptr;
		if(p_obj = dynamic_cast<Object *>(p))
		{
			
			// find depth !!! and sort by it
			
			////	
			
			//~ printf("Object name : %s\n", p_obj->name);
			while(p_obj->getParent() != nullptr){
				//~ printf("\tParent name : %s -- DEPTH --> %d\n", p_obj->getParent()->name, depths[i].second);
				p_obj = (Object *)p_obj->getParent();
				depths[i].second += 1;
				
			}				
		}
		
		
		
		
	}
	
	sort(depths.begin(), depths.end(), []( std::pair<Entity3D*, int > pair1, std::pair<Entity3D*, int > pair2){
		
		return pair1.second > pair2.second;
	});
	
	struct TREE_ITEM{
		Entity3D* obj;
		std::vector<Entity3D*> parents;
	};
	
	std::vector<TREE_ITEM> tree_items;
	for (int i = 0; i < depths.size(); i++)
	{
		Entity3D * p = depths[i].first;
		Object   * p_object = nullptr;
		
		std::vector<Entity3D*> parents;
		
		
		if(p_object = dynamic_cast<Object*>(p)){
			
			Object * cur = p_object;
			while(cur->getParent()){
				printf("((((( %s\n", cur->getParent()->name);
				
				//insert in first place 
				parents.insert(parents.begin(),(Entity3D*)cur->getParent());

				cur = (Object*)cur->getParent();

			}
		}		

		
		tree_items.push_back({depths[i].first, parents});
	}
	
	for (int i = 0; i < tree_items.size(); i++)
	{
		TREE_ITEM item = tree_items[i];
		printf("Tree item : \n");
		printf("\tname : %s\n", item.obj->name);
		for (int pa = 0; pa < item.parents.size(); pa++)
		{
			Object* obj = (Object*)(item.parents[pa]);
			if(obj != nullptr){
				
			//~ printf("\t\t parent %d : %s\n" , pa, obj->name);
			//~ printf("\t\t parent %d \n" , pa);
			
			}else{
				//~ printf("\t\tnullptr !!!!!!\n");
			}
		}
		
	}
	

	
}

void Window::objectListDialog()
{
	

	ImGui::Begin("Object List");
	
	if( ImGui::Button("save to file") ){
		saveToFile();
	}	
	
	if( ImGui::Button("load from file") ){
		loadFromFile("saved_scene.agn");
	}		
	if(ImGui::ListBoxHeader("", 5))
	{
		for (int i = 0; i < objects.size(); i++)
		{
			char text[500];
			sprintf(text, "Object %d", i);
			if(ImGui::Selectable(objects[i]->name, i == cur_object_selected))
			{
				cur_object_selected = i;
				cur_mesh_filter_selected = 0; // not sure if needed
			}
		}
		ImGui::ListBoxFooter();
	}
	
	
	std::vector<std::string> items = {"Mesh Object", "Dummy Object", "Light"};
	static int choice = 0;
	if(ImGui::BeginCombo("add entity", items[choice].c_str(),0)){
		
		for (int i = 0; i < items.size(); i++)
		{
			if(ImGui::Selectable( items[i].c_str())){
				choice = i;
				
			}
		}
		
		ImGui::EndCombo();
	}
	
	if(ImGui::Button("Add Object"))
	{
		if(choice == 0)
		{
			//~ printf("--- START add object \n");
			Object* obj = new Object();
			
			obj->init();
			obj->setGenerator<FileMesh>();
			obj->generator_type = 3;
			obj->mesh_generator->need_update = true;
			obj->shader = default_shader;
			addObject(obj);
		}else if(choice == 1){
			ObjectDummy* dummy = new ObjectDummy();
			
			dummy->init();
			
			addObject(dummy);			
		}else if(choice == 2){
			ObjectLight* light = new ObjectLight();
			
			light->init();			
			addObject(light);			
		}
		
		//~ printf("--- END add object \n");
		
		cur_object_selected = objects.size()-1;
	}
	
	if(ImGui::Button("Delete Object"))
	{
		if(objects.size() > 0)
			removeObject(objects[cur_object_selected]);
		
	}
	
	if(ImGui::Button("Duplicate Object")){
		if(objects.size() > 0){

			Object * p_object = nullptr;
			ObjectDummy * p_dummy = nullptr;
			
			if(p_object = dynamic_cast<Object*>(objects[cur_object_selected])){
				Object * object = new Object((const Object&)*p_object);
				object->init();

				object->mesh_generator->need_update = true;

				addObject(object);
				object->applyTransforms();
				cur_object_selected = objects.size()-1;

			}else if(p_dummy = dynamic_cast<ObjectDummy*>(objects[cur_object_selected])){
				ObjectDummy * dummy = new ObjectDummy((const ObjectDummy&)*p_dummy);
				dummy->init();
				addObject(dummy);
				dummy->applyTransforms();
				cur_object_selected = objects.size()-1;
				//~ printf("Dummy Name -> %s\n", dummy->name);
			}
				
			
			
		}
	}
	ImGui::End();
}

void Window::evalKeyframes()
{
	for (int i = 0; i < objects.size(); i++)
	{
		Entity3D * cur_entity = objects[i];
		Object *   ptr_object = nullptr;
		
		// check transforms
		
		ParamVec3 * p_pos = nullptr;
		ParamVec3 * p_rot = nullptr;
		ParamVec3 * p_scale = nullptr;
		
		
		if(p_pos = dynamic_cast<ParamVec3 *>(cur_entity->p_pos)){
			p_pos->setValue( glm::vec3( p_pos->param_x->getValueAtFrame(time_line.current_frame), p_pos->param_y->getValueAtFrame(time_line.current_frame), p_pos->param_z->getValueAtFrame(time_line.current_frame) ));
			cur_entity->applyTransforms();
		}
		if(p_rot = dynamic_cast<ParamVec3 *>(cur_entity->p_rot)){
			p_rot->setValue( glm::vec3( p_rot->param_x->getValueAtFrame(time_line.current_frame), p_rot->param_y->getValueAtFrame(time_line.current_frame), p_rot->param_z->getValueAtFrame(time_line.current_frame) ));
			cur_entity->applyTransforms();
		}
		if(p_scale = dynamic_cast<ParamVec3 *>(cur_entity->p_scale)){
			p_scale->setValue( glm::vec3( p_scale->param_x->getValueAtFrame(time_line.current_frame), p_scale->param_y->getValueAtFrame(time_line.current_frame), p_scale->param_z->getValueAtFrame(time_line.current_frame) ));
			cur_entity->applyTransforms();
		}		
		if(ptr_object = dynamic_cast<Object *>(cur_entity))
		{
			// check mesh generator
			for (int param_id = 0; param_id < ptr_object->mesh_generator->param_layout.getSize(); param_id++)
			{
				BaseParam * ptr = ptr_object->mesh_generator->param_layout.getParam(param_id);
				ParamFloat * ptr_float = nullptr;

				
				if(ptr_float = dynamic_cast<ParamFloat *>(ptr))
				{
					ptr_float->setValue( ptr_float->getValueAtFrame(time_line.current_frame));
					ptr_object->mesh_generator->need_update = true;
				}
			
			}
			
			// check mesh filters
			for (int filter_id = 0; filter_id < ptr_object->meshFilters.size(); filter_id++)
			{
				MeshFilter * cur_filter = ptr_object->meshFilters[filter_id];
				for (int param_id= 0; param_id <  cur_filter->param_layout.getSize(); param_id++)
				{
					BaseParam * p = cur_filter->param_layout.getParam(param_id);
					ParamFloat * ptr_float = nullptr;
					ParamVec3 * ptr_vec3 = nullptr;
					
					if(ptr_float = dynamic_cast<ParamFloat *>(p)){
						ptr_float->setValue( ptr_float->getValueAtFrame(time_line.current_frame));
						cur_filter->need_update = true;						
					}	
					if(ptr_vec3 = dynamic_cast<ParamVec3 *>(p))
					{
						ptr_vec3->param_x->setValue( ptr_vec3->param_x->getValueAtFrame(time_line.current_frame));
						ptr_vec3->param_y->setValue( ptr_vec3->param_y->getValueAtFrame(time_line.current_frame));
						ptr_vec3->param_z->setValue( ptr_vec3->param_z->getValueAtFrame(time_line.current_frame));
						cur_filter->need_update = true;	
					}									
				}				
			}			
		}		
	}	
}

void Window::buildParamUiKeyframePopupBegin(BaseParam * param)
{

		
	if(param->getNumKeyframes() > 0)
	{
		if(param->isKeyframe(time_line.current_frame))
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0.3f, 1.0f, 0.3f, 1.0f));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.3f, 0.3f, 1.0f));
	}	
		
}

void Window::buildParamUiKeyframePopupEnd(BaseParam * param, std::function<void()> callback, int vec3_ID)
{
	if(param->getNumKeyframes() > 0)
	{
		ImGui::PopStyleColor();
	}	
	
			
	
	if(vec3_ID != -1)
	{
		std::string __name = param->getName();
		if(vec3_ID == 0){
			__name += "_x";			
		}else if(vec3_ID == 1){
			__name += "_y";
		}else if(vec3_ID == 2){
			__name += "_z";
		}
			
		//~ printf("adding ::: %s\n", __name.c_str());
		ImGui::PushID(__name.c_str());					
	}else{
				
		ImGui::PushID(param->getName().c_str());
	}
	if (ImGui::BeginPopupContextItem("item context menu"))
	{
		ParamFloat   * p_float_2  = nullptr;
		ParamInt     * p_int_2   = nullptr;
		ParamString  * p_string_2 = nullptr;
		ParamAction  * p_action_2 = nullptr;
		ParamBool 	 * p_bool_2   = nullptr;
		ParamMenu    * p_menu_2   = nullptr;
		ParamVec3    * p_vec3_2   = nullptr;
		
		if (ImGui::Selectable("Remove All Keyframes")){
			param->removeAllKeyframes();
		}
		if( ImGui::Selectable("Remove Keyframe")) {
			param->removeKeyframeAtFrame(time_line.current_frame);
		}
		if (ImGui::Selectable("Add Keyframe")){
			
			if(p_float_2 = dynamic_cast<ParamFloat *>(param)){
				
				Keyframe<float>* key = new Keyframe<float>();
				
				key->setFrame((float)time_line.current_frame);
				key->setValue(p_float_2->getValue());
				param->addKeyframe(key);
				callback();
			}else if(p_int_2 = dynamic_cast<ParamInt *>(param)){
				
				Keyframe<int>* key = new Keyframe<int>();
				
				key->setFrame((int)time_line.current_frame);
				key->setValue(p_int_2->getValue());
				param->addKeyframe(key);
				callback();
			}
			
			std::vector<BaseKeyframe*> keys = param->getKeyframes();
			for (int i = 0; i < keys.size(); i++)
			{
				//~ printf("key %d : %.3f \n", i, keys[i]->getFrame());
			}
			
		}
		ImGui::PushItemWidth(-1);
		
		ImGui::PopItemWidth();
		ImGui::EndPopup();
		
		
		
	}					
	
	ImGui::PopID();					
}

void Window::buildParamUi(BaseParam * param, std::function<void()> callback)
{
			
	ParamFloat   * p_float  = nullptr;
	ParamInt     * p_int    = nullptr;
	ParamString  * p_string = nullptr;
	ParamAction  * p_action = nullptr;
	ParamBool 	 * p_bool   = nullptr;
	ParamMenu    * p_menu   = nullptr;
	ParamVec3    * p_vec3   = nullptr;
	
	//~ if(param->getNumKeyframes() > 0)
	//~ {
		//~ if(param->isKeyframe(time_line.current_frame))
			//~ ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(0.3f, 1.0f, 0.3f, 1.0f));
		//~ else
			//~ ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.3f, 0.3f, 1.0f));
	//~ }			
	
	if(p_vec3 = dynamic_cast<ParamVec3*>(param))
	{
		ImGui::Text(param->getName().c_str());
		ImGui::Columns(3);
		// X
		buildParamUiKeyframePopupBegin(p_vec3->param_x);
		
		
		if(ImGui::DragFloat(p_vec3->param_x->getName().c_str(), &(p_vec3->param_x->value)))
		{
			if(p_vec3->param_x->isKeyframe(time_line.current_frame))
			{
				BaseKeyframe * cur_key = p_vec3->param_x->getKeyframe(time_line.current_frame);
				
				if(cur_key)
				{
					Keyframe<float>* key_float = nullptr;
					if(key_float = dynamic_cast<Keyframe<float> *>(cur_key))
					{
						//~ printf("setting value for frame %d\n", time_line.current_frame);
						key_float->setValue(p_vec3->param_x->getValue());
					}
				}
			}	
			callback();
		}						
		
		buildParamUiKeyframePopupEnd(p_vec3->param_x, callback, 0);
		
		ImGui::NextColumn();
		// Y
		buildParamUiKeyframePopupBegin(p_vec3->param_y);
		
		//~ _name = p_vec3->getName();
		//~ _name += p_vec3->param_y->getName();		
		if(ImGui::DragFloat(p_vec3->param_y->getName().c_str(), &(p_vec3->param_y->value)))
		{
			if(p_vec3->param_y->isKeyframe(time_line.current_frame))
			{
				BaseKeyframe * cur_key = p_vec3->param_y->getKeyframe(time_line.current_frame);
				
				if(cur_key)
				{
					Keyframe<float>* key_float = nullptr;
					if(key_float = dynamic_cast<Keyframe<float> *>(cur_key))
					{
						//~ printf("setting value for frame %d\n", time_line.current_frame);
						key_float->setValue(p_vec3->param_y->getValue());
					}
				}
			}	
			callback();
		}						
		
		buildParamUiKeyframePopupEnd(p_vec3->param_y, callback, 1);		
		
		
		// Z	
		ImGui::NextColumn();	
		buildParamUiKeyframePopupBegin(p_vec3->param_z);
		
		//~ _name = p_vec3->getName();
		//~ _name += p_vec3->param_z->getName();		
		if(ImGui::DragFloat(p_vec3->param_z->getName().c_str(), &(p_vec3->param_z->value)))
		{
			if(p_vec3->param_z->isKeyframe(time_line.current_frame))
			{
				BaseKeyframe * cur_key = p_vec3->param_z->getKeyframe(time_line.current_frame);
				
				if(cur_key)
				{
					Keyframe<float>* key_float = nullptr;
					if(key_float = dynamic_cast<Keyframe<float> *>(cur_key))
					{
						//~ printf("setting value for frame %d\n", time_line.current_frame);
						key_float->setValue(p_vec3->param_z->getValue());
					}
				}
				
				
			}	
			
			callback();
		}						
		
		buildParamUiKeyframePopupEnd(p_vec3->param_z, callback, 2);		
		
		ImGui::Columns(1);		
	}else{
	
		buildParamUiKeyframePopupBegin(param);
		
		if(p_float = dynamic_cast<ParamFloat*>(param))
		{
			if(ImGui::DragFloat(p_float->getName().c_str(), &p_float->value))
			{
				if(p_float->isKeyframe(time_line.current_frame))
				{
					BaseKeyframe * cur_key = p_float->getKeyframe(time_line.current_frame);
					
					if(cur_key)
					{
						Keyframe<float>* key_float = nullptr;
						if(key_float = dynamic_cast<Keyframe<float> *>(cur_key))
						{
							//~ printf("setting value for frame %d\n", time_line.current_frame);
							key_float->setValue(p_float->getValue());
						}
					}
				}	
				callback();
			}				
		}else if(p_int = dynamic_cast<ParamInt*>(param)){
			int _val = p_int->value;			
			if(ImGui::DragInt(p_int->getName().c_str(), &_val)){
				p_int->setValue(_val);
				callback();
			}				
		}else if(p_string = dynamic_cast<ParamString*>(param)){			
			if(ImGui::InputText(p_string->getName().c_str(), (char*)p_string->value.c_str(), 200))
			{
				callback();	
			}				
		}else if(p_bool = dynamic_cast<ParamBool*>(param)){			
			if(ImGui::CheckboxFlags(p_bool->getName().c_str(), (unsigned int*)&p_bool->value, 1))
			{
				callback();	
			}				
		}else if(p_action = dynamic_cast<ParamAction*>(param)){	
					
			ImGui::Text(p_action->getName().c_str());
			if(ImGui::Button(p_action->getName().c_str())){
				
				p_action->value();
				callback();
			}				
		}
		//~ else if(p_vec3 = dynamic_cast<ParamVec3*>(param)){	
			//~ 
			//~ float vals[3] = {p_vec3->value.x, p_vec3->value.y, p_vec3->value.z};
			//~ if(ImGui::DragFloat3(p_vec3->getName().c_str() , vals, 0.1f)){
				//~ p_vec3->value.x = vals[0];
				//~ p_vec3->value.y = vals[1];
				//~ p_vec3->value.z = vals[2];
				//~ 
				//~ callback();
			//~ }
		//~ }
		else if(p_menu = dynamic_cast<ParamMenu*>(param)){
		
			static int choice = 0;
			if(ImGui::BeginCombo(
					p_menu->getName().c_str(),
					p_menu->getLabels()[choice].c_str(),
					0 )
			)
			{
				for (int i = 0; i < p_menu->getLabels().size(); i++)
				{
					if(ImGui::Selectable(p_menu->getLabels()[i].c_str(), choice == i))
					{								
						choice = i;
						p_menu->setValue(choice);
						
						callback();
					}
				}
				
				ImGui::EndCombo();		
			}
		}
		
		
		buildParamUiKeyframePopupEnd(param, callback);
	}

}

void Window::objectPropertiesDialog()
{	
	ImGui::Begin("Properties");
	
	if( objects.size() == 0 || cur_object_selected == -1){
		ImGui::Text("No Object");
		ImGui::End();		
		
	}else{
		


		char text[500];
		sprintf(text, "object %d", cur_object_selected);
		Entity3D* curEntity = objects[cur_object_selected];
		
		Object * p_object = nullptr;
		ObjectDummy * p_dummy = nullptr;
		if( p_object = dynamic_cast<Object *>(curEntity)){
			
			if(ImGui::Button("to json")){
				json j = p_object->toJSON();
				
				
				
				
				if(p_object->has_generator){
					j["mesh_generator"] = p_object->mesh_generator->toJSON();
				}
				
				std::string s = j.dump(4);
				//~ printf("%s\n", s.c_str());
			}		
		}else if( p_dummy = dynamic_cast<ObjectDummy *>(curEntity)){
			
			if(ImGui::Button("to json")){
				json j = p_dummy->toJSON();
				std::string s = j.dump(4);
				
				//~ printf("%s\n", s.c_str());
			}		
		}
		// current object name
		if( ImGui::InputText(":name" , curEntity->name,IM_ARRAYSIZE(curEntity->name)))
		{

		} 		
		
		ImGui::Columns(1);
		
		if( ImGui::CollapsingHeader("Main Properties"))
		{
		
		
			if( ImGui::BeginTabBar("main_tabs"))
			{			
				if( ImGui::BeginTabItem("Transform"))
				{
					//// transform stuff
					buildParamUi(curEntity->param_layout.getParam(0), [curEntity](){						
						curEntity->applyTransforms();
					});
						
					ImGui::Separator();	
					
					buildParamUi(curEntity->param_layout.getParam(1), [curEntity](){						
						curEntity->applyTransforms();
					});	
						
					ImGui::Separator();	
					
					buildParamUi(curEntity->param_layout.getParam(2), [curEntity](){						
						curEntity->applyTransforms();
					});						
					ImGui::Separator();	
					
					
					//// parenting stufff
					ImGui::Text("Parent");
					ImGui::Columns(2);
					if(curEntity->getParent())
					{	
						Entity3D * parent_ptr =  curEntity->getParent();
						ImGui::Text((const char *)parent_ptr->name);
					}else{
						ImGui::Text("no parent");
					}
					
					ImGui::NextColumn();
					
					
					if(ImGui::BeginCombo("Set Parent","...",0))
					{
						
						for (int i = 0; i < objects.size(); i++)
						{
							//// if not yourself
							if( objects[i]->getID() != curEntity->getID())
							{
								//// if has already a parent , needed , crashing !!
								if(objects[i]->getParent() != nullptr)
								{
									//// finally check if current is already a child of yours in which case
									///// it doesn't make any sense to become the child of your child
									if(curEntity->getID() != objects[i]->getParent()->getID())
									{
										
										if(ImGui::Selectable(objects[i]->name))
										{
											//~ printf("Did I just choose a parent ?\n");
											curEntity->setParent(objects[i]);
										}
									}
								}else{
									if(ImGui::Selectable(objects[i]->name))
									{
										//~ printf("Did I just choose a parent ?\n");
										curEntity->setParent(objects[i]);
									}									
								}
							}
						}
						
						if(ImGui::Selectable("None"))
						{
							curEntity->resetParent();
						}
						
						
					
					
						ImGui::EndCombo();
					}
					
					
					ImGui::Columns(1);
					ImGui::Separator();	
					ImGui::Text("Look At target");
					ImGui::Columns(2);
					
					if(curEntity->getLookAtTarget())
					{	
						Entity3D * target_ptr =  curEntity->getLookAtTarget();
						ImGui::Text((const char *)target_ptr->name);
					}else{
						ImGui::Text("no target");
					}					
					ImGui::NextColumn();
					
					if(ImGui::BeginCombo("Set Look At Target","...",0))
					{
						
						for (int i = 0; i < objects.size(); i++)
						{
							//// if not yourself
							if( objects[i]->getID() != curEntity->getID())
							{
								if(ImGui::Selectable(objects[i]->name))
								{
									//~ printf("Did I just choose a target ?\n");
									objects[i]->applyTransforms();
									curEntity->setLookAtTarget(objects[i]);
									glm::vec3 t_pos = objects[i]->getWorldPosition();
									//~ printf("world pos : %.3f, %.3f, %.3f\n", t_pos.x, t_pos.y, t_pos.z);
								}
							}
						}
						
						if(ImGui::Selectable("None"))
						{
							curEntity->resetLookAtTarget();
						}
						
						
					
					
						ImGui::EndCombo();
					}					
					
					ImGui::Separator();					
					ImGui::EndTabItem();
				}
			
				ImGui::Columns(1);
				Entity3D * p = curEntity;
				Object * curObj = nullptr;
				
				if(curObj = dynamic_cast<Object *>(p))				
				{							
					if( ImGui::BeginTabItem("Mesh"))
					{
						if(ImGui::BeginTabBar("mesh_tabs"))
						{	
							
							if( ImGui::BeginTabItem("Generator"))
							{		
								static bool need_update = false;
								
								static int choice = 0;						
								
								const char* items[] = {"Make a choice ","Sphere Mesh", "Geo Sphere Mesh",  "Grid Mesh", "Box Mesh", "File Mesh", "Cylinder Mesh"};
								
								static int combo_current_item = 0;
								
								
								
								if(curObj->has_generator){
									combo_current_item = curObj->generator_type;
								}else{
									combo_current_item = choice;
								}
								
								const int items_length = 7;
								
								if(ImGui::BeginCombo("Generators",items[combo_current_item],0))
								{
									for (int i = 1; i < items_length; i++)
									{
										if(ImGui::Selectable(items[i], choice == i))
										{								
											choice = i;
											curObj->generator_type = choice;
										}
									}
									
									ImGui::EndCombo();
								}	
									
								if(ImGui::Button("set"))
								{
									if(choice == 1){
										curObj->setGenerator<SphereMesh>();
										curObj->mesh_generator->need_update = true;
									}else if(choice == 2){
										curObj->setGenerator<GeoSphereMesh>();
										curObj->mesh_generator->need_update = true;
									}else if(choice == 3){
										curObj->setGenerator<GridMesh>();
										curObj->mesh_generator->need_update = true;
									}else if(choice == 4){
										curObj->setGenerator<BoxMesh>();
										curObj->mesh_generator->need_update = true;
									}else if(choice == 5){
										curObj->setGenerator<FileMesh>();
										curObj->mesh_generator->need_update = true;					
									}else if(choice == 6){
										curObj->setGenerator<CylinderMesh>();
										curObj->mesh_generator->need_update = true;					
									}
								}
								
								if(curObj->has_generator)
								{
									for(size_t i=0; i< curObj->mesh_generator->param_layout.getSize(); i++)
									{
										BaseParam *p = curObj->mesh_generator->param_layout.getParam(i);
										
										buildParamUi(p, [curObj](){
												curObj->mesh_generator->need_update = true;											
										});											
										
										ImGui::Separator();
									}								
								}
								ImGui::EndTabItem();
							}
								
							if( ImGui::BeginTabItem("Filters"))
							{	
							
								static int choice = 0;
								std::vector<std::string> items = {
										"...", 
										"Transform", 
										"Inflate", 
										"Twist", 
										"Compute Normals", 
										"Spherify", 
										"Duplicate", 
										"From Polar",
										"Mirror"};
								if(ImGui::BeginCombo("filters", items[choice].c_str(), 0))
								{
									for (int i = 1; i < items.size(); i++)
									{
										if(ImGui::Selectable(items[i].c_str(), true))
										{
											choice = i;								
										}
									}
									
									ImGui::EndCombo();
								}
								
								if(ImGui::Button("add filter"))
								{
									if(choice == 1)
									{														
										curObj->hasFilters = true;
										curObj->setMeshFilter<TransformMeshFilter>();
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("transform");
										
									}else if(choice == 2){
										curObj->hasFilters = true;
										curObj->setMeshFilter<InflateMeshFilter>();								
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("inflate");
										
									}else if(choice == 3){
										curObj->hasFilters = true;
										curObj->setMeshFilter<TwistMeshFilter>();				
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("twist");
										
									}else if(choice == 4){
										curObj->hasFilters = true;
										curObj->setMeshFilter<ComputeNormalsMeshFilter>();		
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("compute_normals");
										
									}else if(choice == 5){
										curObj->hasFilters = true;
										curObj->setMeshFilter<SpherifyMeshFilter>();		
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("spherify");
									}else if(choice == 6){
										curObj->hasFilters = true;
										curObj->setMeshFilter<DuplicateMeshFilter>();		
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("duplicate");
									}else if(choice == 7){
										curObj->hasFilters = true;
										curObj->setMeshFilter<FromPolarMeshFilter>();		
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("from_polar");
									}else if(choice == 8){
										curObj->hasFilters = true;
										curObj->setMeshFilter<MirrorMeshFilter>();		
										curObj->meshFilters[ curObj->meshFilters.size()-1]->setName("mirror");
									}
									
								}
								
								ImGui::Separator();
								
								if(ImGui::ListBoxHeader("", 5))
								{
									for (int i = 0; i < curObj->meshFilters.size(); i++)
									{
										
										if(ImGui::Selectable((const char*)curObj->meshFilters[i]->name, cur_mesh_filter_selected == i))
										{
											cur_mesh_filter_selected = i;
										}
									}
									
									ImGui::ListBoxFooter();
								}
								
								//// params layout 
								
								if(curObj->meshFilters.size() > 0)
								{
									char * char_name = curObj->meshFilters[cur_mesh_filter_selected]->name;
									if(ImGui::InputText(
										"name_", 
										curObj->meshFilters[cur_mesh_filter_selected]->name, 
										IM_ARRAYSIZE(curObj->meshFilters[cur_mesh_filter_selected]->name))){
										
									}
										
									
									
									if(ImGui::CheckboxFlags("is active", (unsigned int*)&curObj->meshFilters[cur_mesh_filter_selected]->is_active, 1))
									{
										
											curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											
											// force follwing filters need_update also
											for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
											{
												curObj->meshFilters[i]->need_update = true;
											}
											
									}
									ImGui::SameLine();
									
									if(ImGui::Button("delete filter"))
									{
										
										delete curObj->meshFilters[cur_mesh_filter_selected];
										curObj->meshFilters.erase(curObj->meshFilters.begin() + cur_mesh_filter_selected);
										
										
										if(cur_mesh_filter_selected > 0)
											cur_mesh_filter_selected -= 1;								
											
										if( curObj->meshFilters.size() == 0){
											printf("----------   reset to mesh generator cache --------------- \n");
											curObj->updateMesh();
											//~ curObj->mesh = curObj->mesh_generator->mesh_cache;
											//~ curObj->mesh_generator->need_update = true;
											curObj->hasFilters = false;
										}
										// force following filters need_update also
										for (int i = cur_mesh_filter_selected; i < curObj->meshFilters.size(); i++)
										{
											curObj->meshFilters[i]->need_update = true;
										}
										
										printf("Deleted Mesh Filter \n");									
											
									}								
									ImGui::Columns(2);
									if(ImGui::Button("move down"))
									{
										if(cur_mesh_filter_selected != curObj->meshFilters.size()-1)
										{	
											curObj->moveFilter(cur_mesh_filter_selected, cur_mesh_filter_selected+1);
											cur_mesh_filter_selected++;
											
											curObj->meshFilters[cur_mesh_filter_selected-1]->need_update = true;
											
											// force follwing filters need_update also
											for (int i = cur_mesh_filter_selected; i < curObj->meshFilters.size(); i++)
											{
												curObj->meshFilters[i]->need_update = true;
											}											
										}
									}
									ImGui::NextColumn();
									if(ImGui::Button("move up"))
									{
										if(cur_mesh_filter_selected != 0)
										{
											curObj->moveFilter(cur_mesh_filter_selected, cur_mesh_filter_selected-1);
											cur_mesh_filter_selected--;
											curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											
											// force follwing filters need_update also
											for (int i = cur_mesh_filter_selected; i < curObj->meshFilters.size(); i++)
											{
												curObj->meshFilters[i]->need_update = true;
											}												
										}
									}
									ImGui::Columns(1);									
								}
								
								if( curObj->meshFilters.size() > 0)
								{
									for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->param_layout.getSize(); i++)
									{
										BaseParam *p = curObj->meshFilters[cur_mesh_filter_selected]->param_layout.getParam(i);
										
										buildParamUi(p, [this, curObj](){
											curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											// force follwing filters need_update also
											for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
											{
												curObj->meshFilters[i]->need_update = true;
											}											
										});
									}								
								}								
								ImGui::EndTabItem();
							}							
							ImGui::EndTabBar();
						}						
						ImGui::EndTabItem();
					}					
					ImGui::Separator();
					
					
					if( ImGui::BeginTabItem("Material"))
					{
						if( ImGui::Button("Reload Shader"))
						{
							//~ printf("reloading shader now \n");
							curObj->initShader();
						}
						
						
						ImGui::ColorEdit3("u_color", (float*)&curObj->color);
						ImGui::EndTabItem();
					}						
			
				}		
				
				ImGui::EndTabBar();
			}
		
		}
		if (ImGui::CollapsingHeader("Display Options"))
		{	
			Object * p_object = nullptr;
			
			if(p_object = dynamic_cast<Object*>(objects[cur_object_selected]))
			{
				ImGui::CheckboxFlags("Display Polygons", (unsigned int*)&p_object->bDisplayPolygons, 1);
				ImGui::CheckboxFlags("Display Bbox", (unsigned int*)&p_object->bDisplayBoundingBox, 1);
				ImGui::CheckboxFlags("Display Points", (unsigned int*)&p_object->bDisplayPoints, 1);
				ImGui::CheckboxFlags("Display Wireframe", (unsigned int*)&p_object->bDisplayWireframe, 1);
				ImGui::CheckboxFlags("Display Normals", (unsigned int*)&p_object->bDisplayNormals, 1);
			}
		}		
			
		ImGui::End();
	}
}

void Window::drawKeyframes(BaseParam* _param, int selected_key_id)
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	const ImVec2 p = ImGui::GetCursorScreenPos();
	ImVec2 size = ImGui::GetWindowSize();	
	std::vector<BaseKeyframe*> cur_keys = _param->keyframes;
	for (int key_ID = 0; key_ID < cur_keys.size(); key_ID++)
	{
		
		BaseKeyframe * cur_key = cur_keys[key_ID];
		Keyframe<float> * key_float = nullptr;
		Keyframe<int> * key_int = nullptr;
		
		if( key_float = dynamic_cast<Keyframe<float> *>(cur_key)){
			
			
			float frame = key_float->getFrame();
			float value = key_float->getValue();
			
			//~ printf("Key frame -> %.3f, value -> %.3f\n", frame, value);
			
			ImVec2 cursor_pos = p;
			float start_x = cursor_pos.x;
			float start_y = cursor_pos.y;
			float max_x = start_x+ size.x - 11.0f;
			float size_y = 11.0f;
			float key_pos_x = cursor_pos.x + (max_x - cursor_pos.x) * ((frame-(float)time_line.start) / ((float)time_line.end - (float)time_line.start));
			draw_list->AddRectFilled(
				ImVec2(key_pos_x, p.y), 
				ImVec2(key_pos_x+11.0f, start_y + size_y), 
				selected_key_id == key_ID ? ImColor(ImVec4(0.9f,0.2f,0.9f,1.0f)) : ImColor(ImVec4(0.2f,0.2f,0.9f,1.0f)));		
							
			if(key_ID != cur_keys.size()-1)
			{
				Keyframe<float> * key_float_after = dynamic_cast<Keyframe<float> *>(cur_keys[key_ID+1]);

				float frame_after = key_float_after->getFrame();
				float value_after = key_float_after->getValue();					
				
				float key_after_pos_x = cursor_pos.x + (max_x - cursor_pos.x) * ((frame_after-(float)time_line.start) / ((float)time_line.end - (float)time_line.start));

				const ImVec2 pos0 = ImVec2(key_pos_x, start_y + (value / 20.0f) * size_y);
				const ImVec2 cp0 = ImVec2(key_pos_x +30.0f, start_y + (value / 20.0f) * size_y);
				const ImVec2 cp1 = ImVec2(key_after_pos_x - 30.0f, start_y + (value_after / 20.0f) * size_y);
				const ImVec2 pos1 = ImVec2(key_after_pos_x, start_y + (value_after / 20.0f) * size_y);
			
				const ImU32 col = ImColor(ImVec4(1.0f,0.0f,0.0f,1.0f));
				
				draw_list->AddBezierCurve(pos0, cp0, cp1, pos1, col, 2.0f);
				
			} 
		}else if( key_int = dynamic_cast<Keyframe<int> *>(cur_key)){
			
			
			float frame = key_int->getFrame();
			int value = key_int->getValue();
			
			//~ printf("Key frame -> %.3f, value -> %.3f\n", frame, value);
			
			ImVec2 cursor_pos = p;
			float start_x = cursor_pos.x;
			float start_y = cursor_pos.y;
			float max_x = start_x+ size.x - 11.0f;
			float size_y = 11.0f;
			float key_pos_x = cursor_pos.x + (max_x - cursor_pos.x) * ((frame-(float)time_line.start) / ((float)time_line.end - (float)time_line.start));
			draw_list->AddRectFilled(
				ImVec2(key_pos_x, p.y), 
				ImVec2(key_pos_x+11.0f, start_y + size_y), 
				selected_key_id == key_ID ? ImColor(ImVec4(0.9f,0.2f,0.9f,1.0f)) : ImColor(ImVec4(0.2f,0.2f,0.9f,1.0f)));		
							
			if(key_ID != cur_keys.size()-1)
			{
				Keyframe<int> * key_int_after = dynamic_cast<Keyframe<int> *>(cur_keys[key_ID+1]);

				float frame_after = key_int_after->getFrame();
				int value_after = key_int_after->getValue();					
				
				float key_after_pos_x = cursor_pos.x + (max_x - cursor_pos.x) * ((frame_after-(float)time_line.start) / ((float)time_line.end - (float)time_line.start));

				const ImVec2 pos0 = ImVec2(key_pos_x, start_y + ((float)value / 20.0f) * size_y);
				const ImVec2 cp0 = ImVec2(key_pos_x +30.0f, start_y + ((float)value / 20.0f) * size_y);
				const ImVec2 cp1 = ImVec2(key_after_pos_x - 30.0f, start_y + ((float)value_after / 20.0f) * size_y);
				const ImVec2 pos1 = ImVec2(key_after_pos_x, start_y + ((float)value_after / 20.0f) * size_y);
			
				const ImU32 col = ImColor(ImVec4(1.0f,0.0f,0.0f,1.0f));
				
				draw_list->AddBezierCurve(pos0, cp0, cp1, pos1, col, 2.0f);
				
			} 
		}
	
	}	
}

void Window::timeLineDialog()
{
	
	
	
	ImGui::Begin("Timeline");
	

	ImGui::Columns(3);
	if(ImGui::Button("play"))
	{
		time_line.play();		
	}
	ImGui::NextColumn();
	if(ImGui::Button("pause"))
	{
		time_line.pause();		
	}	
	ImGui::NextColumn();
	if(ImGui::Button("stop"))
	{
		time_line.stop();
		evalKeyframes();		
	}
	ImGui::Columns(1);	

	
	ImGui::PushItemWidth(-1);
	if(ImGui::SliderInt("frame", &time_line.current_frame, time_line.start, time_line.end, "%d")){
		evalKeyframes();
	}
	
	
	
	ImGui::Text("%d", time_line.current_frame);
	
	Entity3D * cur_entity = objects[cur_object_selected];
	
	if(objects.size() > 0 && cur_object_selected != -1)
	{
		std::vector<BaseParam *> all_params;
		

		
		// build params list
		
		for(int i=0; i< cur_entity->param_layout.getSize(); i++){
			all_params.push_back(cur_entity->param_layout.getParam(i));

		}
		
		//~ all_params.push_back(cur_entity->p_pos);
		//~ all_params.push_back(cur_entity->p_rot);
		//~ all_params.push_back(cur_entity->p_scale);
		// check if cur_entity is an Object *
		Object * p_object = nullptr;
		if( p_object = dynamic_cast<Object *>(cur_entity)) {
			
			for(int i=0; i< p_object->mesh_generator->param_layout.getSize(); i++){
				all_params.push_back(p_object->mesh_generator->param_layout.getParam(i));
				//~ if(ImGui::Selectable(p_object->mesh_generator->param_layout.getParam(i)->getName().c_str(),false)){
				//~ 
				//~ }
			}		
			
			for (int i = 0; i < p_object->meshFilters.size(); i++)
			{
				
				for (int j = 0; j < p_object->meshFilters[i]->param_layout.getSize(); j++)
				{
					all_params.push_back(p_object->meshFilters[i]->param_layout.getParam(j));
				}
				
			}
					
		}	
		
		static int selected_param = 0;
		
		if( selected_param >= all_params.size() ){
			selected_param = all_params.size()-1;
		}
		
		static int selected_key_id = 0;
		if( selected_key_id > all_params[selected_param]->keyframes.size()-1){
			selected_key_id =  all_params[selected_param]->keyframes.size()-1;
		}
		if( ImGui::BeginCombo("params", all_params[selected_param]->getName().c_str(), 1) ){
			for(int i=0; i< all_params.size(); i++){
				if(ImGui::Selectable(all_params[i]->getName().c_str(),selected_param == i)){
					selected_param = i;
				}
			}

		
			ImGui::EndCombo();
		}
		
		BaseParam * cur_param = all_params[selected_param];		
		
		std::vector<std::string> interpolation_choices = {"LINEAR", "SMOOTHSTEP"};
		static int choice = cur_param->getInterpolationType();
		if(ImGui::BeginCombo("interpolation type", interpolation_choices[cur_param->getInterpolationType()].c_str(), 1))
		{
			
			for (int i = 0; i < interpolation_choices.size(); i++)
			{
				if(ImGui::Selectable(interpolation_choices[i].c_str(), choice == i))
				{
					choice = i;
					ParamVec3 * p_vec3 = nullptr;
					if( choice == 0) {
						
						if( p_vec3 = dynamic_cast<ParamVec3 *>(cur_param)){
							cur_param->setInterpolationType(LINEAR);
							
							p_vec3->param_x->setInterpolationType(LINEAR);
							p_vec3->param_y->setInterpolationType(LINEAR);
							p_vec3->param_z->setInterpolationType(LINEAR);
						}else{
							
							cur_param->setInterpolationType(LINEAR);
						}
					}else if( choice == 1){
						if( p_vec3 = dynamic_cast<ParamVec3 *>(cur_param)){
							cur_param->setInterpolationType(SMOOTHSTEP);
							
							p_vec3->param_x->setInterpolationType(SMOOTHSTEP);
							p_vec3->param_y->setInterpolationType(SMOOTHSTEP);
							p_vec3->param_z->setInterpolationType(SMOOTHSTEP);
						}else{					
							cur_param->setInterpolationType(SMOOTHSTEP);
						}
					}
					
					
				}
			}
			
			
			ImGui::EndCombo();
		}	
			
		

		

		
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		const ImVec2 p = ImGui::GetCursorScreenPos();
		ImVec2 size = ImGui::GetWindowSize();
		
		draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x+size.x - 18.0f, p.y+100.0f), ImColor(ImVec4(1.0f,1.0f,0.5f,1.0f)));		
			

		
		
		ParamFloat * ptr_float = nullptr;
		ParamInt * ptr_int = nullptr;
		ParamVec3 * ptr_vec3  = nullptr;
		
		if(ptr_float = dynamic_cast<ParamFloat *>(cur_param))
		{
			std::vector<BaseKeyframe*> keys = ptr_float->getKeyframes();
			//~ printf("num keyframes = %d\n", keys.size());
			drawKeyframes(ptr_float, selected_key_id);
			
		}else if(ptr_int = dynamic_cast<ParamInt *>(cur_param))
		{
			std::vector<BaseKeyframe*> keys = ptr_int->getKeyframes();
			//~ printf("num keyframes = %d\n", keys.size());
			drawKeyframes(ptr_int, selected_key_id);
			
		}else if(ptr_vec3 = dynamic_cast<ParamVec3 *>(cur_param)){
			
			std::vector< std::vector<BaseKeyframe*> > keys_array;
			
			drawKeyframes(ptr_vec3->param_x, selected_key_id);
			drawKeyframes(ptr_vec3->param_y, selected_key_id);
			drawKeyframes(ptr_vec3->param_z, selected_key_id);


		}

		
		// make invisible area 
		ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
		ImVec2 canvas_size = ImGui::GetContentRegionAvail(); 	
		ImGui::InvisibleButton("invisible_area", ImVec2(canvas_size[0], 100));
		
		if( ImGui::IsItemHovered() ){
			//~ printf("hovered !!!!\n");
		 }	
		
		
		ImGui::Separator();
		
		if(ImGui::Button("P")){
			
			selected_key_id--;
			//~ printf("previous %d\n", selected_key_id);
		}
		ImGui::SameLine();
		if(ImGui::Button("N")){
			
			selected_key_id++;
			//~ printf("next %d\n", selected_key_id);
		}	
		
	}
		ImGui::End();
		
}

void Window::refresh()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	
	 // int ex = explorerDialog();
		
	objectListDialog();
	objectPropertiesDialog();
	timeLineDialog();
	
	ImGui::Render();

	
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0,0,width, height);
	
	
	time_line.update();
	if(time_line.is_playing){
		evalKeyframes();
	}
	
	
	camera.setProjection(
		glm::perspective(camera.fov_angle, (float)width/ (float)height, camera.near, camera.far)
	);
	
	float ratio = (float)width / height;
	
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2,0.2,0.2,1.0);

	
	
	for (int i = 0; i < objects.size(); i++)
	{
		Object * ptr = nullptr;
		if( ptr = dynamic_cast<Object*>(objects[i])){
			
			ptr->updateMesh();
		}
	}
	//~ 
	//~ 
	renderObjects();
	


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	//swap buffers
	
	glfwSwapBuffers(win);
	
	glfwPollEvents();
	
	
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(win);
}

void Window::addObject(Entity3D* obj)
{
	
	for (int i = 0; i < objects.size(); i++)
	{
		//~ std::cout << "OLD NAME : "<<objects[i]->name <<"\n";
		if(strcmp(obj->name, objects[i]->name) == 0) // if equal to zero means strings are equal
		{
			//~ std::cout << "changing name \n";
			std::string newName = objects[i]->name;
			newName += "_";
			strcpy(obj->name, newName.c_str());			
		}		
	}
	
	obj->setID(cur_unique_id);
	cur_unique_id++;
	
	objects.push_back(obj);
	
	//~ printf("added to objects list\n");
	
}

void Window::removeObject(Entity3D* obj)
{
		//// check for children and reset parent to nullptr if needed
		for (int i = 0; i < objects.size(); i++)
		{
			if(objects[i]->getParent() != nullptr){
				
				if(objects[i]->getParent()->getID() == obj->getID())
				{
					//~ printf("got child !!!\n");
					objects[i]->resetParent();
				}
			}
		}
		
		int index_to_remove = findObjectIndexByID(obj->getID());
		delete obj;
		objects.erase(objects.begin() + index_to_remove);
		
		if(cur_object_selected > 0)
			cur_object_selected -= 1;	
			
		if( objects.size() == 0)
		{
			cur_object_selected = -1;
			cur_unique_id = 0;
		}
			
		//~ printf("Objects number is %d \n", objects.size());
}

Entity3D Window::duplicateObject(Entity3D * obj)
{
	
	Entity3D * p = obj;
	
	Object * p_object = nullptr;
	ObjectDummy * p_dummy = nullptr;
	
	//~ 
	if(p_object = dynamic_cast<Object*>(p)){
		
		Object new_object = *(p_object);
		
		
		//~ printf("trying to copy an 'Object'\n");
		//~ printf("param layout size -> %d\n", new_object.param_layout.getSize());
		//~ 
		
		new_object.setName("copied_object");
		
		new_object.setID( cur_unique_id++);
		
		cur_object_selected = objects.size();
		
		return new_object;
	}else if(p_dummy = dynamic_cast<ObjectDummy*>(p)){
		ObjectDummy dum = *(p_dummy);
		//~ printf("trying to copy a 'Dummy'\n");
		//~ dum.init();
		dum.setID( cur_unique_id++);
		cur_unique_id += 1;		
		return dum;
	}

	
	Entity3D null_entity;
	
	return null_entity;
}

void Window::renderObjects()
{
		glEnable(GL_DEPTH_TEST);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 model =  glm::mat4(1.0f);
		//~ projection*= glm::perspective(45.0f, (float)width / (float)height, 0.01f, 100.0f);

		
		glm::vec3 up_vector = glm::vec3(0.0f,0.0f,1.0f);

		view *= glm::lookAt(
								camera.position, 
								camera.target_position, 
								glm::normalize(up_vector)
							);			


		
		// draw world grid
				point_shader.useProgram();
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));	
										
				GLuint COLOR_LOC = glGetUniformLocation(point_shader.m_id,"u_color");
				glUniform4f(COLOR_LOC, 1.0, 1.0, 1.0, 0.5);	
				
				drawWorldGrid();
				
				glUseProgram(0);
		////

	
	for (int i = 0; i < objects.size(); i++)
	{	
		//~ printf("RENDERING ENTITY %d -----\n", i);
		Entity3D* curEntity = objects[i];
		model = glm::mat4(1.0f);
		
		

		//// apply own transforms
		model = curEntity->transforms * model;
		
		if(curEntity->getLookAtTarget() != nullptr)
		{
			Entity3D * target = curEntity->getLookAtTarget();
			model *= glm::inverse(glm::lookAt(
				curEntity->getWorldPosition(),
				target->getWorldPosition(),
				glm::vec3(0.0f, 0.0f, 1.0f)));
		}						
		//// apply parents transforms
		curEntity->applyParentsMatrices(model);
		
	
		
	
	
		

		
		Object      * curObj        = nullptr;
		ObjectDummy * curDummy = nullptr;
		if( curObj = dynamic_cast<Object *>(objects[i]))
		{			
			curObj->shader.useProgram();	

			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));	
									
			GLuint COLOR_LOC = glGetUniformLocation(curObj->shader.m_id,"u_color");

			if(cur_object_selected == i)
				glUniform4f(COLOR_LOC, 1.0, 1.0, 0.2, 1.0);
			else
				glUniform4f(COLOR_LOC, curObj->color.x, curObj->color.y, curObj->color.z, curObj->color.w);
			//~ curObj->texture.bind();
			
			glUniform1i(glGetUniformLocation(curObj->shader.m_id, "u_tex"),0);

			//~ if(cur_object_selected == i)
				//~ drawSelGizmo();
			
			if(curObj->bDisplayPolygons){			
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				curObj->draw(curObj->getRenderMode());
			}
				
			if(curObj->bDisplayWireframe)
			{
				wireframe_shader.useProgram();
				glUniformMatrix4fv(glGetUniformLocation(wireframe_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));	
				glUniformMatrix4fv(glGetUniformLocation(wireframe_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(wireframe_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));					

				glUniform3f(glGetUniformLocation(wireframe_shader.m_id,"u_camera_pos"), camera.position.x, camera.position.y, camera.position.z);	
				COLOR_LOC = glGetUniformLocation(wireframe_shader.m_id,"u_color");
				glPointSize(5);
				glUniform4f(COLOR_LOC, 0.0,0.0,1.0,1.0);
				
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				
					curObj->draw(curObj->getRenderMode());				
					
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			}
			
			if(curObj->bDisplayPoints){
				point_shader.useProgram();
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));					
											
				COLOR_LOC = glGetUniformLocation(point_shader.m_id,"u_color");
				glUniform4f(COLOR_LOC, 1.0,0.0,0.0,1.0);			
				curObj->drawPoints();
			}			
			
			
			if(curObj->bDisplayNormals){
											
				COLOR_LOC = glGetUniformLocation(curObj->shader.m_id,"u_color");
				glUniform4f(COLOR_LOC, 1.0,0.0,0.0,1.0);			
				curObj->drawNormals();
			}
			
			if(curObj->bDisplayBoundingBox)
			{
				//~ glUseProgram(0);
				point_shader.useProgram();
				COLOR_LOC = glGetUniformLocation(point_shader.m_id,"u_color");
				glPointSize(5);
				glUniform4f(COLOR_LOC, 1.0,1.0,0.0,1.0);				
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));				
				curObj->drawBoundingBox();
			}
			
		}
		 
		if(curDummy = dynamic_cast<ObjectDummy *>(objects[i]))
		{			
			point_shader.useProgram();

			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));	
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniform4f(
				glGetUniformLocation(point_shader.m_id,"u_color"), 
				1.0,1.0,0.0,1.0);				
			curDummy->draw();	
			
			glUseProgram(0);
		}
	}

	glDisable(GL_DEPTH_TEST);
	
	for (int i = 0; i < gizmos.size(); i++)
	{
		
		if( cur_object_selected != -1){
			
			model = glm::mat4(1.0f);
			
			model = objects[cur_object_selected]->transforms * model;
			
			objects[cur_object_selected]->applyTransforms();
			objects[cur_object_selected]->applyParentsMatrices(model);
			
			
			gizmos[i]->target_object = objects[cur_object_selected];
			gizmos[i]->transforms = model;
			
			point_shader.useProgram();
			
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));	
			//~ glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));	
									
			//~ GLuint COLOR_LOC = glGetUniformLocation(point_shader.m_id,"u_color");
			//~ 
			//~ glUniform4f(COLOR_LOC, 1.0, 0.0, 0.0, 1.0);			
			gizmos[i]->draw(point_shader, camera);
			
			glUseProgram(0);
		}
	}
	
}

void Window::saveToFile()
{
	std::ofstream out_file; 
	json main_json;
	std::vector<json> entities;
	for (int i = 0; i < objects.size(); i++)
	{
		
	
	
		Entity3D* curEntity = objects[i];
		
		curEntity->applyTransforms();
		Object * p_object = nullptr;
		ObjectDummy * p_dummy = nullptr;
		if( p_object = dynamic_cast<Object *>(curEntity)){
			
			
			json j = p_object->toJSON();	
			

			//~ if(p_object->has_generator){
				//~ j["mesh_generator"] = p_object->mesh_generator->toJSON();
			//~ }
			
			entities.push_back(j);
			//~ std::string s = j.dump(4);
			//~ printf("%s\n", s.c_str());
		
		}else if( p_dummy = dynamic_cast<ObjectDummy *>(curEntity)){
			
			json j = p_dummy->toJSON();
			
			//~ std::string s = j.dump(4);
			
			entities.push_back(j);
			//~ printf("%s\n", s.c_str());
	
		}	
	}
	
	main_json["entities"] = entities;
	
	out_file.open("saved_scene.agn");
	out_file << main_json.dump(4);
	out_file.close();
}

void Window::loadFromFile(std::string file_path)
{
	objects.clear();
	cur_unique_id = 0;
	
	std::ifstream in_file(file_path);
	std::string line;
	std::string s;
	
	json j;
  if (in_file.is_open())
  {
    while ( getline (in_file,line) )
    {
		s += line;
    }
    in_file.close();
  }	
  
  j = json::parse(s);
  
  //~ std::cout << j.dump(2).c_str();
  //~ 
  //~ 
  //~ std::cout << "\n";
  
  
  for (int i = 0; i < j["entities"].size(); i++)
  {
	  json cur_j = j["entities"][i];
	  
	  if( cur_j["type"].get<std::string>() == "OBJECT"){
		  
		
		//~ printf("building new OBJECT\n");
		Object * new_obj = new Object();
		new_obj->fromJSON(cur_j, default_shader);
		addObject(new_obj);
		  
		  
		  
	  }else if( cur_j["type"].get<std::string>() == "OBJECT_DUMMY"){
		  //~ printf("building new OBJECT DUMMY\n");
		ObjectDummy * new_obj = new ObjectDummy();
		new_obj->fromJSON(cur_j);
		addObject(new_obj);
	  }
  }
  
  
  // all entities are created, now I can deal with parenting
  
  for (int i = 0; i < objects.size(); i++)
  {
	  Entity3D * cur_entity = objects[i];
	  json cur_j = j["entities"][i];
	  //~ printf("parent ID is --> %d \n", (cur_j["parent"].get<int>()));
	  if( cur_j["parent"].get<int>() != -1)
	  {
		  //~ printf("parent ID is --> %d \n", cur_j["parent"].get<int>());
		  //~ cur_entity->setParent( objects[findObjectIndexByID( cur_j["parent"].get<int>())]);
	  }
  }
  
  
  evalKeyframes();
	
}

Window::~Window()
{
	
}


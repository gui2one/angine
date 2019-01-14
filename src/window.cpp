#include "window.h"

#include <iostream>
#include <typeinfo>
#include <sstream>
#include <algorithm>

#include <array>

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

////

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
	//~ glm::vec3 hitP = glm::vec3(0.0f);
	//~ int hit = ray_plane_intersect(
			//~ glm::vec3(0.0f,0.0f,-1.0f),// plane normal
			//~ glm::vec3(0.0f,0.0f,0.0f),// plane point
			//~ glm::vec3(0.1f,2.2f,2.0f),// point pos
			//~ glm::vec3(0.0f,0.0f,-3.0f),// raydir
			//~ hitP);
	//~ printf("hit : %s\n", (hit == 1 ? "true":"false"));
	//~ if( hit){
			//~ printf("ray hit !\n");
			//~ printf("\tpos : %.3f, %.3f, %.3f\n", hitP.x, hitP.y, hitP.z);
			//~ printf("-----------------\n");
	//~ }
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
	

	
	//~ io.Fonts->Build();
	
	
	const char* glsl_version = "#version 130";

	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	

	initWorldGrid();
	

	setCamPosFromPolar(camera_u_pos, camera_v_pos);

	// default first object
	Object* obj = new Object();
	
	obj->init();
	obj->shader = default_shader;
	obj->setName("wheel_1");
	obj->setGenerator<CylinderMesh>();
	
	std::vector<BaseKeyframe> keyframes;
	
	Keyframe<float> key_1;
	keyframes.push_back(key_1);
	obj->mesh_generator->param_layout.getParam(0)->setKeyframes(keyframes);
	obj->rotation.y = -45.0;
	obj->applyTransforms();
	obj->generator_type = 6; // really crappy design , do something !!!!
	obj->mesh_generator->need_update = true;
	
	addObject(obj);
	
	
	ObjectDummy * dummy = new ObjectDummy();
	dummy->setName("null_1");
	dummy->init();
	
	dummy->position.x = -1.5;
	dummy->applyTransforms();
	addObject(dummy);
	//~ Object* obj2 = new Object();
	//~ 
	//~ obj2->init();
	//~ obj2->setParent(obj);
	//~ obj2->setGenerator<BoxMesh>();
	//~ obj2->position.x = 2.0;
	//~ obj2->applyTransforms();	
	//~ obj2->generator_type = 6; // really crappy design , do something !!!!
	//~ obj2->mesh_generator->need_update = true;
	//~ 
	//~ addObject(obj2);	
	//~ 
	//~ Object* obj3 = new Object();
	//~ 
	//~ obj3->init();
	//~ obj3->setParent(obj2);
	//~ obj3->setGenerator<CylinderMesh>();
	//~ obj3->generator_type = 6; // really crappy design , do something !!!!
	//~ obj3->mesh_generator->need_update = true;
	//~ 
	//~ addObject(obj3);		

	//~ selGizmoInit();

}

void Window::initWorldGrid()
{

	//// build verts
	//~ struct vert{
		//~ float x, y, z;
	//~ };
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
		//~ printf("camera_u_pos : %.3f\n", app->camera_u_pos);
		//~ printf("camera_v_pos : %.3f\n", app->camera_v_pos);
		//~ printf("---------------------------\n");
		app->camera_u_pos -= app->mouse_delta_x * rot_speed;
		

			
		app->camera_v_pos += app->mouse_delta_y * rot_speed;
		
		if(app->camera_v_pos < 0.2)
			app->camera_v_pos = 0.2;
		else if(app->camera_v_pos > PI-0.2)
			app->camera_v_pos = PI-0.2;		
		
		app->setCamPosFromPolar(app->camera_u_pos, app->camera_v_pos);
		
	}
	
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Window* app = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS ){
		
		app->right_mouse_button_down = true;
		
		glfwGetCursorPos(window, &app->mouse_pos_x, &app->mouse_pos_y);
		app->mouse_old_x = app->mouse_pos_x;
		app->mouse_old_y = app->mouse_pos_y;
		
		
	}else if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE ){
		
		app->right_mouse_button_down = false;
		
	}
	
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
		printf("Sending ray, sir !\n");
		
		double x_pos, y_pos;
		glfwGetCursorPos(window, &x_pos, &y_pos);
		
		float x = (2.0f * x_pos) / app->width - 1.0f;
		float y = 1.0f - (2.0f * y_pos) / app->height;
		glm::vec3 planeN = glm::vec3(0.0f, 0.0f , 1.0f);
		glm::vec3 planeP = glm::vec3(0.0f, 0.0f , 0.0f);
		glm::vec3 pointP = glm::vec3(x, y , 1.0f);
		glm::vec3 rayDir = glm::vec3(0.0f, 0.0f , -2.0f);
		
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		projection*= glm::perspective(45.0f, (float)app->width / (float)app->height, 0.01f, 100.0f);
		//~ projection[3][2] = 0.0f;
		projection[3][3] = 1.0f; // not sure why I need this, but it gets rid off a nasty offset 
		
		glm::vec3 up_vector = glm::vec3(0.0f,0.0f,1.0f);

		view *= glm::lookAt(
								app->camera.position, 
								app->camera.target_position, 
								glm::normalize(up_vector)
							);			
							
		glm::vec4 tempPointP = inverse(projection * view)  * glm::vec4(pointP.x, pointP.y, pointP.z, 1.0f) ;
		tempPointP /= tempPointP.w ;
		
		
		//~ printf("new pointP : \n\t%.3f, %.3f, %.3f\n", tempPointP.x, tempPointP.y, tempPointP.z);
		
		glm::vec3 hitP = glm::vec3(0.0f);
		int hit = ray_plane_intersect(planeN, planeP, app->camera.position, tempPointP, hitP);
		
		if( hit)
		{
			printf("ray hit at : %.3f, %.3f,%.3f\n", hitP.x, hitP.y, hitP.z);
			
			ObjectDummy * dum = new ObjectDummy();
			dum->position = hitP;
			dum->applyTransforms();
			dum->init();
			app->addObject(dum);
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



void Window::setCamPosFromPolar(float u, float v)
{
	camera.position.x = sin(u)* sin(v) * 5.0;
	camera.position.y = cos(u)* sin(v) *5.0;
	camera.position.z = cos(v) * 5.0;	
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
			
			printf("Object name : %s\n", p_obj->name);
			while(p_obj->getParent() != nullptr){
				printf("\tParent name : %s -- DEPTH --> %d\n", p_obj->getParent()->name, depths[i].second);
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
		
		//~ printf("<<<<<<<<<<<<<< %s\n", cur->name);
		if(p_object = dynamic_cast<Object*>(p)){
			
			Object * cur = p_object;
			while(cur->getParent()){
				printf("((((( %s\n", cur->getParent()->name);
				
				//insert in first place 
				parents.insert(parents.begin(),(Entity3D*)cur->getParent());
				
				//~ parents.push_back((Object*)cur->getParent());
				
				
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
				
			printf("\t\t parent %d : %s\n" , pa, obj->name);
			//~ printf("\t\t parent %d \n" , pa);
			
			}else{
				printf("\t\tnullptr !!!!!!\n");
			}
		}
		
	}
	

	
}

void Window::objectListDialog()
{
	ImGui::Begin("Object List");
	
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
			printf("--- START add object \n");
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
		
		printf("--- END add object \n");
		
		cur_object_selected = objects.size()-1;
	}
	
	if(ImGui::Button("Delete Object"))
	{
		removeObject(objects[cur_object_selected]);
		//~ delete objects[cur_object_selected];
		//~ objects.erase(objects.begin() + cur_object_selected);
		//~ 
		//~ if(cur_object_selected > 0)
			//~ cur_object_selected -= 1;
	}
	ImGui::End();
}

void Window::objectPropertiesDialog()
{	
	ImGui::Begin("Properties");
	
	if( objects.size() == 0){
		ImGui::Text("No Object");
		ImGui::End();		
		
	}else{

		char text[500];
		sprintf(text, "object %d", cur_object_selected);
		Entity3D* curEntity = objects[cur_object_selected];
		
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
					glm::mat4 temp_matrix = glm::mat4(1.0f);
					
					ImGui::LabelText("", "Position");
					ImGui::Columns(3,"columns");
					
					if(ImGui::DragFloat(":tx", &curEntity->position.x)){
						curEntity->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":ty", &curEntity->position.y)){
						curEntity->applyTransforms();						
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":tz", &curEntity->position.z)){
						curEntity->applyTransforms();					
					}
					
					ImGui::Separator();
					
					ImGui::Columns(1);			
					ImGui::LabelText("", "Rotation");
					ImGui::Columns(3,"columns");
					if(ImGui::DragFloat(":rx", &curEntity->rotation.x)){						
						curEntity->applyTransforms();					
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":ry", &curEntity->rotation.y)){
						curEntity->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":rz", &curEntity->rotation.z)){
						curEntity->applyTransforms();
					}
					
					ImGui::Separator();		
					
					ImGui::Columns(1);			
					ImGui::LabelText("", "Scale");
					ImGui::Columns(3,"columns");
					if(ImGui::DragFloat(":sx", &curEntity->scale.x)){
						curEntity->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":sy", &curEntity->scale.y)){
						curEntity->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":sz", &curEntity->scale.z)){
						curEntity->applyTransforms();
					}
					ImGui::Columns(1);	
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
						ImGui::Text("no parent");
					}					
					ImGui::NextColumn();
					
					if(ImGui::BeginCombo("Set Look At Target","...",0))
					{
						
						for (int i = 0; i < objects.size(); i++)
						{
							//// if not yourself
							if( objects[i]->getID() != curEntity->getID())
							{
								//// if has already a target , needed , crashing !!
								//~ if(objects[i]->getParent() != nullptr)
								//~ {
									//~ //// finally check if current is already a target of yours in which case
									//~ ///// it doesn't make any sense to become the target of your child
									//~ if(curEntity->getID() != objects[i]->getLookAtTarget()->getID())
									//~ {
										//~ 
										//~ if(ImGui::Selectable(objects[i]->name))
										//~ {
											//~ printf("Did I just choose a target ?\n");
											//~ curEntity->setLookAtTarget(objects[i]);
										//~ }
									//~ }
								//~ }else{
									if(ImGui::Selectable(objects[i]->name))
									{
										printf("Did I just choose a target ?\n");
										curEntity->setLookAtTarget(objects[i]);
									}									
								//~ }
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
										
										
										//~ printf("WTF !!!!!!!!!!!!!!!!!!\n");
										BaseParam *p = curObj->mesh_generator->param_layout.getParam(i);
										
										ParamFloat *p_float = nullptr;
										ParamInt *p_int = nullptr;
										ParamString *p_string = nullptr;
										ParamBool *p_bool = nullptr;
										ParamAction *p_action = nullptr;
										
										if(p_float = dynamic_cast<ParamFloat *>(p))
										{

						


											if(p_float->getNumKeyframes() > 0)
											{
												ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor(1.0f, 0.3f, 0.3f, 1.0f));
											}											
											if(ImGui::DragFloat(p_float->getName().c_str(), &p_float->value))
											{
												
												curObj->mesh_generator->need_update = true;	
											}
											
											if(p_float->getNumKeyframes() > 0)
											{
												ImGui::PopStyleColor();
											}	
											
												
												ImGui::PushID(p_float->getName().c_str());
												if (ImGui::BeginPopupContextItem("item context menu"))
												{
													if (ImGui::Selectable("Remove All Keyframes")){
														p_float->removeAllKeyframes();
													}
													if (ImGui::Selectable("Add Keyframe")){
														Keyframe<float> key;
														p_float->addKeyframe(key);
													}
													ImGui::PushItemWidth(-1);
													//~ ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
													ImGui::PopItemWidth();
													ImGui::EndPopup();
													
												}					
												
												ImGui::PopID();							
										

											
																						
											//~ if(ImGui::OpenPopupOnItemClick("item context menu", 1))
											//~ {
												//~ 
												//~ 
											//~ }
											
														
										}
										if(p_int = dynamic_cast<ParamInt *>(p))
										{	
											int _val = p_int->value;									
											if(ImGui::DragInt(p_int->getName().c_str(), &_val))
											{
												p_int->setValue(_val);
												curObj->mesh_generator->need_update = true;	
											}
										}									
										if(p_string = dynamic_cast<ParamString *>(p))
										{											
											if(ImGui::InputText(p_string->getName().c_str(), (char*)p_string->value.c_str(), 200))
											{
												curObj->mesh_generator->need_update = true;	
											}
										}	
										if(p_bool = dynamic_cast<ParamBool*>(p))
										{
											//~ ImGui::Text(p_bool->getName().c_str());
											if(ImGui::CheckboxFlags(p_bool->getName().c_str(), (unsigned int*)&p_bool->value, 1)){
												printf(" bool param : %s\n", (p_bool->getValue()==true ? "true": "false"));
												curObj->mesh_generator->need_update = true;	
											}
										
										}	
										if(p_action = dynamic_cast<ParamAction*>(p))
										{
											ImGui::Text(p_action->getName().c_str());
											if(ImGui::Button(p_action->getName().c_str())){
												
												p_action->value();
												curObj->mesh_generator->need_update = true;	
											}
										
										}									
							
										
										ImGui::Separator();
									}
																	
								}
								ImGui::EndTabItem();
							}
								
							if( ImGui::BeginTabItem("Filters"))
							{	
							
								static int choice = 0;
								std::vector<std::string> items = {"...", "Transform", "Inflate", "Twist", "Compute Normals", "Spherify", "Duplicate", "From Polar"};
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
											curObj->moveFilter(cur_mesh_filter_selected, cur_mesh_filter_selected+1);
									}
									ImGui::NextColumn();
									if(ImGui::Button("move up"))
									{
										if(cur_mesh_filter_selected != 0)
											curObj->moveFilter(cur_mesh_filter_selected, cur_mesh_filter_selected-1);
									}
									ImGui::Columns(1);									
								}
								
								if( curObj->meshFilters.size() > 0)
								{
									for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->param_layout.getSize(); i++)
									{
										BaseParam *p = curObj->meshFilters[cur_mesh_filter_selected]->param_layout.getParam(i);
										
										ParamFloat *p_float = nullptr;
										ParamVec3 *p_vec3 = nullptr;
										ParamInt *p_int = nullptr;
										ParamBool *p_bool = nullptr;
										ParamAction *p_action = nullptr;
										ParamMenu *p_menu = nullptr;
										
										if(p_float = dynamic_cast<ParamFloat*>(p))
										{
											if(ImGui::DragFloat( p_float->getName().c_str(), &p_float->value))
											{
												curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
												// force follwing filters need_update also
												for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
												{
													curObj->meshFilters[i]->need_update = true;
												}
											}
										}
										
										if(p_int = dynamic_cast<ParamInt*>(p))
										{
											if(ImGui::DragInt( p_int->getName().c_str(), &p_int->value))
											{
												curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
												// force follwing filters need_update also
												for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
												{
													curObj->meshFilters[i]->need_update = true;
												}
											}
										}									
										
										if(p_vec3 = dynamic_cast<ParamVec3*>(p))
										{
											char text[500];
											sprintf(text, "%sx", p_vec3->prefix.c_str());										
											ImGui::Text(p_vec3->getName().c_str());
											ImGui::Columns(3,"columns");
											if(ImGui::DragFloat( text, &p_vec3->value.x))
											{
												curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
												// force follwing filters need_update also
												for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
												{
													curObj->meshFilters[i]->need_update = true;
												}
											}
											
											ImGui::NextColumn();
											sprintf(text, "%sy", p_vec3->prefix.c_str());
											if(ImGui::DragFloat( text, &p_vec3->value.y))
											{
												curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
												// force follwing filters need_update also
												for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
												{
													curObj->meshFilters[i]->need_update = true;
												}
											}
											ImGui::NextColumn();
											sprintf(text, "%sz", p_vec3->prefix.c_str());
											if(ImGui::DragFloat( text, &p_vec3->value.z))
											{
												curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
												// force follwing filters need_update also
												for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
												{
													curObj->meshFilters[i]->need_update = true;
												}
											}										
											
											ImGui::Columns(1);
										}									
										
										if(p_bool = dynamic_cast<ParamBool*>(p))
										{
											//~ ImGui::Text(p_bool->getName().c_str());
											if(ImGui::CheckboxFlags(p_bool->getName().c_str(), (unsigned int*)&p_bool->value, 1)){
												printf(" bool param : %s\n", (p_bool->getValue()==true ? "true": "false"));
												
												curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
												
												// force follwing filters need_update also
												for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
												{
													curObj->meshFilters[i]->need_update = true;
												}
											}
										
										}	
										
										if(p_menu = dynamic_cast<ParamMenu*>(p))
										{
											
											static int choice = 0;
											if(ImGui::BeginCombo(
													p_menu->getName().c_str(),
													p_menu->getValue()[choice].c_str(),
													0 )
											)
											{
												for (int i = 0; i < p_menu->getValue().size(); i++)
												{
													if(ImGui::Selectable(p_menu->getValue()[i].c_str(), choice == i))
													{								
														choice = i;
														p_menu->current_choice = choice;
														
														curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
														// force follwing filters need_update also
														for (int i = cur_mesh_filter_selected+1; i < curObj->meshFilters.size(); i++)
														{
															curObj->meshFilters[i]->need_update = true;
														}													
														//curObj->generator_type = choice;
													}
												}
												
												ImGui::EndCombo();
											}										
							
									}										
									
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
							printf("reloading shader now \n");
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

void Window::timeLineDialog()
{
	ImGui::Begin("Timeline");
	
	//~ ImDrawList* draw_list = ImGui::GetWindowDrawList();
	//~ const ImVec2 p = ImGui::GetCursorScreenPos();
	//~ draw_list->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x+5.0f, p.y+5.0f), ImColor(ImVec4(1.0f,1.0f,0.5f,1.0f)));	
	
	if(ImGui::Button("play"))
	{


	}
	ImGui::End();
}

void Window::refresh()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiIO &io = ImGui::GetIO();
	if(io.WantCaptureMouse){
		
		//~ printf("ImGui captured mouse\n");
	}
	
	 // int ex = explorerDialog();
		
	objectListDialog();
	objectPropertiesDialog();
	timeLineDialog();
	
	ImGui::Render();

	
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0,0,width, height);
	
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
		std::cout << "OLD NAME : "<<objects[i]->name <<"\n";
		if(strcmp(obj->name, objects[i]->name) == 0) // if equal to zero means strings are equal
		{
			std::cout << "changing name \n";
			std::string newName = objects[i]->name;
			newName += "_";
			strcpy(obj->name, newName.c_str());
			
		}
		
	}
	
	obj->setID(cur_unique_id);
	cur_unique_id++;
	
	objects.push_back(obj);
	
}

void Window::removeObject(Entity3D* obj)
{
		//// check for children and reset parent to nullptr if needed
		for (int i = 0; i < objects.size(); i++)
		{
			if(objects[i]->getParent() != nullptr){
				
				if(objects[i]->getParent()->getID() == obj->getID())
				{
					printf("got child !!!\n");
					objects[i]->resetParent();
				}
			}
		}
		
		int index_to_remove = findObjectIndexByID(obj->getID());
		delete obj;
		objects.erase(objects.begin() + index_to_remove);
		
		if(cur_object_selected > 0)
			cur_object_selected -= 1;	
}

void Window::renderObjects()
{
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 model =  glm::mat4(1.0f);
	projection*= glm::perspective(45.0f, (float)width / (float)height, 0.01f, 100.0f);

	
	glm::vec3 up_vector = glm::vec3(0.0f,0.0f,1.0f);
	//~ if( camera.target_position.z > camera.position.z){						
		//~ up_vector =	glm::vec3(0.0f,-1.0f,0.0f);
	//~ }else{
		//~ up_vector =	glm::vec3(0.0f,1.0f,0.0f);
	//~ }
	//~ glm::mat4 ModelViewMatrix = glm::mat4(1.0f); 
	view *= glm::lookAt(
							camera.position, 
							camera.target_position, 
							glm::normalize(up_vector)
						);			


	
	//~ printf("---- START render objects function\n");
	
	// draw world grid
			point_shader.useProgram();
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));	
									
			GLuint COLOR_LOC = glGetUniformLocation(point_shader.m_id,"u_color");
			glUniform4f(COLOR_LOC, 1.0, 1.0, 1.0, 0.5);	
			
			drawWorldGrid();
			
			glUseProgram(0);
	
	for (int i = 0; i < objects.size(); i++)
	{	

		Entity3D* curEntity = objects[i];
		model = glm::mat4(1.0f);
		//// apply own transforms
		model = curEntity->transforms * model;			
		//// apply parents transforms
		curEntity->applyParentsMatrices(model);
		
	
			
		if(curEntity->getLookAtTarget() != nullptr)
		{

		}	
		
		Object      * curObj        = nullptr;
		ObjectDummy * curDummy = nullptr;
		if( curObj = dynamic_cast<Object *>(objects[i]))
		{
			

			curObj->shader.useProgram();	

			
			

			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));	
									
			GLuint COLOR_LOC = glGetUniformLocation(curObj->shader.m_id,"u_color");
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
				point_shader.useProgram();
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));					

				glUniform3f(glGetUniformLocation(point_shader.m_id,"u_camera_pos"), camera.position.x, camera.position.y, camera.position.z);	
				COLOR_LOC = glGetUniformLocation(point_shader.m_id,"u_color");
				glPointSize(5);
				glUniform4f(COLOR_LOC, 0.0,0.0,1.0,1.0);
				
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				
					curObj->draw(curObj->getRenderMode());				
					
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			}
			
			if(curObj->bDisplayPoints){
				point_shader.useProgram();
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
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
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));				
				curObj->drawBoundingBox();
			}
			
		}
		 
		if(curDummy = dynamic_cast<ObjectDummy *>(objects[i]))
		{
			
			point_shader.useProgram();
			
			
		//~ 
			//~ std::vector<Entity3D*> parents  = curDummy->getParents();
			//~ for (int i = 0; i < parents.size(); i++)
			//~ {
				//~ model = parents[i]->transforms * model;
			//~ }
			//~ 
			//~ //// lastly apply own transforms
			//~ model = model * curDummy->transforms;		
			
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(point_shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniform4f(
				glGetUniformLocation(point_shader.m_id,"u_color"), 
				1.0,1.0,0.0,1.0);				
			curDummy->draw();	
			
			glUseProgram(0);
		}
		
		
		
		
		
		
	}
		
		
		//~ printf("---- END render objects function\n");
}

Window::~Window()
{
	
}


#include "window.h"

#include <iostream>
#include <typeinfo>
#include <sstream>

#include <array>

#include "object.h"

// for explorerDialog
#include <dirent.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <algorithm>
////

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
	printf("--- START loading shader\n");
	pointShader.loadVertexShaderSource("../src/res/shaders/line_shader.vert");
	pointShader.loadFragmentShaderSource("../src/res/shaders/line_shader.frag");		
	pointShader.createShader();	
	printf("--- END loading shader\n");
	glfwSwapInterval(1);
	glEnable(GL_DEPTH_TEST);
	
	//~ glEnable(GL_LINE_SMOOTH);
	//~ glEnable(GL_POLYGON_SMOOTH);
	
	glEnable(GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	

	
	//~ io.Fonts->Build();
	
	
	const char* glsl_version = "#version 130";

	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	

	


	// default first object
	Object* obj = new Object();
	
	obj->init();
	obj->setGenerator<CylinderMesh>();
	obj->position.x = 1.0;
	obj->applyTransforms();
	obj->generator_type = 6; // really crappy design , do something !!!!
	obj->mesh_generator->need_update = true;
	
	addObject(obj);
	
	Object* obj2 = new Object();
	
	obj2->init();
	obj2->setParent(obj);
	obj2->setGenerator<BoxMesh>();
	obj2->generator_type = 6; // really crappy design , do something !!!!
	obj2->mesh_generator->need_update = true;
	
	addObject(obj2);	

	selGizmoInit();

}


void Window::selGizmoInit()
{
	
	float vert_data[8*3] = {
			0.00, 0.00, 0.00,
			0.5, 0.00, 0.00,
			0.00, 0.5, 0.00,
			0.00, 0.00, 0.5,

			1.00, 0.00, 0.00,
			0.5, 0.00, 0.00,
			1.00, 0.5, 0.00,
			1.00, 0.00, 0.5			
	};
	glDeleteBuffers(1, &sel_gizmo_vbo);
	glGenBuffers(1, &sel_gizmo_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sel_gizmo_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8*3, vert_data ,GL_DYNAMIC_DRAW);	
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Window::drawSelGizmo(){
	
	//~ pointShader.useProgram();
	glBindBuffer(GL_ARRAY_BUFFER, sel_gizmo_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); 					
	glEnableVertexAttribArray(0);		
	
		glDrawArrays(GL_POINTS,0, 8*3);
	
	glDisableVertexAttribArray(0);		
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	
	//~ glUseProgram(0);		
}

int Window::explorerDialog(){
	
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


void Window::buildObjectList()
{
	for (int i = 0; i < objects.size(); i++)
	{
		printf("Object %d, name : %s\n", i, objects[i]->name);
		if(objects[i]->getParent()){
			printf("\tParent %d, name : %s\n", i, objects[i]->getParent()->name);
		}
		
		// first go through the list and find all objects with a parent
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
	
	
	if(ImGui::Button("Add Object"))
	{
		printf("--- START add object \n");
		Object* obj = new Object();
		
		obj->init();
		obj->setGenerator<BoxMesh>();
		obj->generator_type = 3;
		obj->mesh_generator->need_update = true;
		
		addObject(obj);
		
		printf("--- END add object \n");
		
		cur_object_selected = objects.size()-1;
	}
	
	if(ImGui::Button("Delete Object"))
	{
		delete objects[cur_object_selected];
		objects.erase(objects.begin() + cur_object_selected);
		
		if(cur_object_selected > 0)
			cur_object_selected -= 1;
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
		Object* curObj = objects[cur_object_selected];
		
		// current object name
		if( ImGui::InputText(":name" , curObj->name,IM_ARRAYSIZE(curObj->name)))
		{

		} 		
		
		ImGui::Columns(1);
		
		if( ImGui::CollapsingHeader("Main Properties"))
		{
		
		
			if( ImGui::BeginTabBar("main_tabs"))
			{			
				if( ImGui::BeginTabItem("Transform"))
				{
					glm::mat4 temp_matrix = glm::mat4(1.0f);
					
					ImGui::LabelText("", "Position");
					ImGui::Columns(3,"columns");
					
					if(ImGui::DragFloat(":tx", &objects[cur_object_selected]->position.x)){
						objects[cur_object_selected]->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":ty", &objects[cur_object_selected]->position.y)){
						objects[cur_object_selected]->applyTransforms();						
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":tz", &objects[cur_object_selected]->position.z)){
						objects[cur_object_selected]->applyTransforms();					
					}
					
					ImGui::Separator();
					
					ImGui::Columns(1);			
					ImGui::LabelText("", "Rotation");
					ImGui::Columns(3,"columns");
					if(ImGui::DragFloat(":rx", &objects[cur_object_selected]->rotation.x)){						
						objects[cur_object_selected]->applyTransforms();					
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":ry", &objects[cur_object_selected]->rotation.y)){
						objects[cur_object_selected]->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":rz", &objects[cur_object_selected]->rotation.z)){
						objects[cur_object_selected]->applyTransforms();
					}
					
					ImGui::Separator();		
					
					ImGui::Columns(1);			
					ImGui::LabelText("", "Scale");
					ImGui::Columns(3,"columns");
					if(ImGui::DragFloat(":sx", &objects[cur_object_selected]->scale.x)){
						objects[cur_object_selected]->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":sy", &objects[cur_object_selected]->scale.y)){
						objects[cur_object_selected]->applyTransforms();
					}
					ImGui::NextColumn();
					if(ImGui::DragFloat(":sz", &objects[cur_object_selected]->scale.z)){
						objects[cur_object_selected]->applyTransforms();
					}
					
					ImGui::Separator();	
					ImGui::EndTabItem();
				}
			
				ImGui::Columns(1);
				
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
										if(ImGui::DragFloat(p_float->getName().c_str(), &p_float->value))
										{
											curObj->mesh_generator->need_update = true;	
										}
									}
									if(p_int = dynamic_cast<ParamInt *>(p))
									{										
										if(ImGui::DragInt(p_int->getName().c_str(), &p_int->value))
										{
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
							std::vector<std::string> items = {"...", "Transform", "Inflate", "Twist", "Compute Normals", "Spherify", "Duplicate"};
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
								

								//~ 
								//~ if(curObj->meshFilters.size() > 0)
								//~ {		
									//~ if( ImGui::InputText("name", (char *)(curObj->meshFilters[cur_mesh_filter_selected]->getName().c_str()), 200)){
										//~ std::cout<< curObj->meshFilters[cur_mesh_filter_selected]->getName().c_str() << "\n";
									//~ }
								//~ }
								//~ 
								//~ for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat.size(); i++)
								//~ {		
									//~ if(ImGui::DragFloat(
										//~ curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat[i].name.c_str(),
										//~ &curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat[i].value ))
									//~ {
										//~ std::cout << curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat[i].value << "\n";
										//~ curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										//~ 
										//~ // force follwing filters need_update also
										//~ for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										//~ {
											//~ curObj->meshFilters[j]->need_update = true;
										//~ }									
									//~ }
									//~ 
								//~ }	
								//~ 
								//~ // vec3 params
								//~ for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3.size(); i++)
								//~ {	
									//~ 
										//~ ImGui::LabelText("", curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].name.c_str());
										//~ ImGui::Columns(3,"columns");
										//~ if(ImGui::DragFloat(":x", &curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.x)){
											//~ curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											//~ 
											//~ // force follwing filters need_update also
											//~ for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
											//~ {
												//~ curObj->meshFilters[j]->need_update = true;
											//~ }										
										//~ }
										//~ ImGui::NextColumn();
										//~ if(ImGui::DragFloat(":y", &curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.y)){
											//~ curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											//~ 
											//~ // force follwing filters need_update also
											//~ for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
											//~ {
												//~ curObj->meshFilters[j]->need_update = true;
											//~ }																				
										//~ }
										//~ ImGui::NextColumn();
										//~ if(ImGui::DragFloat(":z", &curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.z)){
											//~ curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											//~ 
											//~ // force follwing filters need_update also
											//~ for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
											//~ {
												//~ curObj->meshFilters[j]->need_update = true;
											//~ }																				
										//~ }
										//~ 
										//~ ImGui::Separator();								
										//~ ImGui::Columns(1);	
										//~ 
									//~ if(ImGui::DragFloat(
										//~ curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].name.c_str(),
										//~ &curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.x ))
									//~ {
										//~ std::cout << curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.x << "\n";
										//~ curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										//~ 
										//~ // force follwing filters need_update also
										//~ for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										//~ {
											//~ curObj->meshFilters[j]->need_update = true;
										//~ }									
									//~ }
									//~ 
								//~ }														
								//~ 
								//~ for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsInt.size(); i++)
								//~ {
									//~ static bool need_update = false;
									//~ 
									//~ if(ImGui::DragInt(
										//~ curObj->meshFilters[cur_mesh_filter_selected]->paramsInt[i].name.c_str(),
										//~ &curObj->meshFilters[cur_mesh_filter_selected]->paramsInt[i].value ))
									//~ {
										//~ std::cout << curObj->meshFilters[cur_mesh_filter_selected]->paramsInt[i].value << "\n";
										//~ curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										//~ 
										//~ // force follwing filters need_update also
										//~ for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										//~ {
											//~ curObj->meshFilters[j]->need_update = true;
										//~ }										
									//~ }
									//~ 
								//~ }
//~ 
								//~ for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsBool.size(); i++)
								//~ {
									//~ 
									//~ if(ImGui::CheckboxFlags("Invert Normals", (unsigned int*)&curObj->meshFilters[cur_mesh_filter_selected]->paramsBool[i].value, 1))
									//~ {
										//~ curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										//~ 
										//~ // force follwing filters need_update also
										//~ for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										//~ {
											//~ curObj->meshFilters[j]->need_update = true;
										//~ }									
									//~ }
								//~ }
								
							}
							ImGui::EndTabItem();
						}
						

						ImGui::EndTabBar();
					}
					
					ImGui::Separator();
					ImGui::EndTabItem();
					
					
			
				}		
				
				if( ImGui::BeginTabItem("Materials"))
				{
					if( ImGui::Button("Reload Shader"))
					{
						printf("reloading shader now \n");
						curObj->initShader();
					}
					
					
					ImGui::ColorEdit3("u_color", (float*)&curObj->color);
					ImGui::EndTabItem();
				}	
					
									
				
				ImGui::EndTabBar();
			}
		
		}
		if (ImGui::CollapsingHeader("Display Options"))
		{	
			ImGui::CheckboxFlags("Display Polygons", (unsigned int*)&objects[cur_object_selected]->bDisplayPolygons, 1);
			ImGui::CheckboxFlags("Display Bbox", (unsigned int*)&objects[cur_object_selected]->bDisplayBoundingBox, 1);
			ImGui::CheckboxFlags("Display Points", (unsigned int*)&objects[cur_object_selected]->bDisplayPoints, 1);
			ImGui::CheckboxFlags("Display Wireframe", (unsigned int*)&objects[cur_object_selected]->bDisplayWireframe, 1);
			ImGui::CheckboxFlags("Display Normals", (unsigned int*)&objects[cur_object_selected]->bDisplayNormals, 1);
		}		
			
		ImGui::End();
	}
}

void Window::refresh(){
	


	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	
	
	
	 // int ex = explorerDialog();
		
	objectListDialog();
	objectPropertiesDialog();

	
	ImGui::Render();

	
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0,0,width, height);
	
	float ratio = (float)width / height;
	
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2,0.2,0.2,1.0);

	
	
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->updateMesh();
	}
	
	
	renderObjects();
	


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	//swap buffers
	
	glfwSwapBuffers(win);
	
	glfwPollEvents();
	
	
}

bool Window::shouldClose(){
	return glfwWindowShouldClose(win);
}

void Window::addObject(Object* obj)
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
	
	
	objects.push_back(obj);
	
}

void Window::renderObjects(){
	
		//~ printf("---- START render objects function\n");
		for (int i = 0; i < objects.size(); i++)
		{	
		
			Object* curObj = objects[i];
			
			curObj->shader.useProgram();	
			
				
			//~ glLoadIdentity();
			
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 model = glm::mat4(1.0f);
			projection*= glm::perspective(45.0f, (float)width / (float)height, 0.01f, 100.0f);

			
			glm::vec3 up_vector;
			if( camera.target_position.z > camera.position.z){						
				up_vector =	glm::vec3(0.0f,-1.0f,0.0f);
			}else{
				up_vector =	glm::vec3(0.0f,1.0f,0.0f);
			}
			//~ glm::mat4 ModelViewMatrix = glm::mat4(1.0f); 
			view *= glm::lookAt(
									camera.position, 
									camera.target_position, 
									glm::normalize(up_vector)
								);			


		
			
			glLoadIdentity();
			
			
			// apply transform matrices
			if(curObj->getParent() != nullptr)
			{
				model = curObj->getParent()->transforms * curObj->transforms;
			}else{
				
				model = curObj->transforms;
			}
			

			
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));	
			
			
			
			
			
									
									
			GLuint COLOR_LOC = glGetUniformLocation(curObj->shader.m_id,"u_color");
			glUniform4f(COLOR_LOC, curObj->color.x, curObj->color.y, curObj->color.z, curObj->color.w);
			
			
			
			//~ curObj->texture.bind();
			
			glUniform1i(glGetUniformLocation(curObj->shader.m_id, "u_tex"),0);

			//~ glLoadIdentity();

			//~ glPointSize(5);
			//~ glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			
			if(cur_object_selected == i)
				drawSelGizmo();
			
			if(curObj->bDisplayPolygons){			
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				curObj->draw(curObj->getRenderMode());
			}
				
			if(curObj->bDisplayWireframe){
				curObj->lineShader.useProgram();
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));					

				glUniform3f(glGetUniformLocation(curObj->lineShader.m_id,"u_camera_pos"), camera.position.x, camera.position.y, camera.position.z);	
				COLOR_LOC = glGetUniformLocation(curObj->lineShader.m_id,"u_color");
				glPointSize(5);
				glUniform4f(COLOR_LOC, 0.0,0.0,1.0,1.0);
				
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				
					curObj->draw(curObj->getRenderMode());				
					
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			}
			
			if(curObj->bDisplayPoints){
				curObj->lineShader.useProgram();
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));					
											
				COLOR_LOC = glGetUniformLocation(curObj->lineShader.m_id,"u_color");
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
				curObj->lineShader.useProgram();
				COLOR_LOC = glGetUniformLocation(curObj->lineShader.m_id,"u_color");
				glPointSize(5);
				glUniform4f(COLOR_LOC, 1.0,1.0,0.0,1.0);				
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
				glUniformMatrix4fv(glGetUniformLocation(curObj->lineShader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));				
				curObj->drawBoundingBox();
			}
			
			
			
		}
		
		
		//~ printf("---- END render objects function\n");
}

Window::~Window()
{
	
}


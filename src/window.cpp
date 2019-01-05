#include "window.h"

#include <iostream>
#include <typeinfo>
#include <sstream>

#include <array>

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
	
	pointShader.loadVertexShaderSource("../src/res/shaders/line_shader.vert");
	pointShader.loadFragmentShaderSource("../src/res/shaders/line_shader.frag");		
	pointShader.createShader();	
	
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
	obj->generator_type = 3;
	obj->mesh_generator->need_update = true;
	
	addObject(obj);



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
		if(strcmp(sd -> d_name,".") == 0 || strcmp(sd -> d_name,"..") == 0){
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
				

				//~ 
				//~ std::cout << "Going up ?!! "<<current_explorer_path<< "\n";
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

void Window::objectListDialog()
{
	ImGui::Begin("Object List");
	
	//~ static int listbox_item_current = 0;
	if(ImGui::ListBoxHeader("", 5))
	{
		for (int i = 0; i < objects.size(); i++)
		{
			char text[500];
			sprintf(text, "Object %d", i);
			if(ImGui::Selectable(objects[i]->name, i == cur_object_selected))
			{
				//~ std::cout << "press item " << i << "\n";
				cur_object_selected = i;
				cur_mesh_filter_selected = 0; // not sure if needed
			}
			
		}
		
		ImGui::ListBoxFooter();
	}
	
	
	if(ImGui::Button("Add Object"))
	{
		Object* obj = new Object();
		
		obj->init();
		obj->setGenerator<BoxMesh>();
		obj->generator_type = 3;
		obj->mesh_generator->need_update = true;
		
		addObject(obj);
		
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
			std::cout << "edited name\n";
		} 		

		
		ImGui::Columns(1);
		
		if( ImGui::CollapsingHeader("Main Properties"))
		{
		
		
			if( ImGui::BeginTabBar("main_tabs"))
			{			
				if( ImGui::BeginTabItem("Transform"))
				{
					ImGui::LabelText("", "Position");
					ImGui::Columns(3,"columns");
					ImGui::DragFloat(":tx", &objects[cur_object_selected]->position.x);
					ImGui::NextColumn();
					ImGui::DragFloat(":ty", &objects[cur_object_selected]->position.y);
					ImGui::NextColumn();
					ImGui::DragFloat(":tz", &objects[cur_object_selected]->position.z);
					
					ImGui::Separator();
					
					ImGui::Columns(1);			
					ImGui::LabelText("", "Rotation");
					ImGui::Columns(3,"columns");
					ImGui::DragFloat(":rx", &objects[cur_object_selected]->rotation.x);
					ImGui::NextColumn();
					ImGui::DragFloat(":ry", &objects[cur_object_selected]->rotation.y);
					ImGui::NextColumn();
					ImGui::DragFloat(":rz", &objects[cur_object_selected]->rotation.z);
					
					ImGui::Separator();		
					
					ImGui::Columns(1);			
					ImGui::LabelText("", "Scale");
					ImGui::Columns(3,"columns");
					ImGui::DragFloat(":sx", &objects[cur_object_selected]->scale.x);
					ImGui::NextColumn();
					ImGui::DragFloat(":sy", &objects[cur_object_selected]->scale.y);
					ImGui::NextColumn();
					ImGui::DragFloat(":sz", &objects[cur_object_selected]->scale.z);
					
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
								for (int i = 0; i < curObj->mesh_generator->paramsInt.size(); i++)
								{
									static int* test_int = &curObj->mesh_generator->paramsInt[i].value;
									if(ImGui::DragInt(curObj->mesh_generator->paramsInt[i].name.c_str(), &curObj->mesh_generator->paramsInt[i].value ))
									{										
										curObj->mesh_generator->need_update = true;									
									}
								}
								
								for (int i = 0; i < curObj->mesh_generator->paramsFloat.size(); i++)
								{
									static float* test_int = &curObj->mesh_generator->paramsFloat[i].value;
									if(ImGui::DragFloat(curObj->mesh_generator->paramsFloat[i].name.c_str(), &curObj->mesh_generator->paramsFloat[i].value ))
									{																	
										curObj->mesh_generator->need_update = true;																	
									}
								}
								
								for (int i = 0; i < curObj->mesh_generator->paramsString.size(); i++)
								{
									
									if(ImGui::InputText(curObj->mesh_generator->paramsString[i].name.c_str(), (char*)curObj->mesh_generator->paramsString[i].value.c_str(),200))
									{
										printf("%s\n", curObj->mesh_generator->paramsString[i].value.c_str());																
									}
								}							
								
								for (int i = 0; i < curObj->mesh_generator->paramsAction.size(); i++)
								{
									
									if(ImGui::Button(curObj->mesh_generator->paramsAction[i].name.c_str()))
									{
										curObj->mesh_generator->paramsAction[i].value();							
										curObj->mesh_generator->need_update = true;																		
									}
								}								

							}
							ImGui::EndTabItem();
						}
						
						if( ImGui::BeginTabItem("Filters"))
						{	
							
							static int choice = 0;
							std::vector<std::string> items = {"...", "Transform", "Inflate", "Twist", "Compute Normals", "Spherify"};
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
									
								}
								
							}
							
							ImGui::Separator();
							
							if(ImGui::ListBoxHeader("", 5))
							{
								for (int i = 0; i < curObj->meshFilters.size(); i++)
								{
									if(ImGui::Selectable(curObj->meshFilters[i]->getName().c_str(), cur_mesh_filter_selected == i))
									{
										cur_mesh_filter_selected = i;
									}
								}
								
								ImGui::ListBoxFooter();
							}
							
							//// params layout 
							
							if(curObj->meshFilters.size() > 0)
							{
								if(ImGui::CheckboxFlags("is active", (unsigned int*)&curObj->meshFilters[cur_mesh_filter_selected]->is_active, 1))
								{
									//~ if(curObj->meshFilters[cur_mesh_filter_selected]->is_active)
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
									printf("delete filter now\n");
									delete curObj->meshFilters[cur_mesh_filter_selected];
									curObj->meshFilters.erase(curObj->meshFilters.begin() + cur_mesh_filter_selected);
									
									
									if(cur_mesh_filter_selected > 0)
										cur_mesh_filter_selected -= 1;								
										
									if( curObj->meshFilters.size() == 0){
										curObj->mesh = curObj->mesh_generator->mesh_cache;
									}
									// force following filters need_update also
									for (int i = cur_mesh_filter_selected; i < curObj->meshFilters.size(); i++)
									{
										curObj->meshFilters[i]->need_update = true;
									}									
										
								}
								
								if(curObj->meshFilters.size() > 0)
								{		
									if( ImGui::InputText("name", (char *)(curObj->meshFilters[cur_mesh_filter_selected]->getName().c_str()), 200)){
										std::cout<< curObj->meshFilters[cur_mesh_filter_selected]->getName().c_str() << "\n";
									}
								}
								
								for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat.size(); i++)
								{		
									if(ImGui::DragFloat(
										curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat[i].name.c_str(),
										&curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat[i].value ))
									{
										std::cout << curObj->meshFilters[cur_mesh_filter_selected]->paramsFloat[i].value << "\n";
										curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										
										// force follwing filters need_update also
										for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										{
											curObj->meshFilters[j]->need_update = true;
										}									
									}
									
								}	
								
								// vec3 params
								for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3.size(); i++)
								{	
									
										ImGui::LabelText("", curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].name.c_str());
										ImGui::Columns(3,"columns");
										if(ImGui::DragFloat(":x", &curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.x)){
											curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											
											// force follwing filters need_update also
											for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
											{
												curObj->meshFilters[j]->need_update = true;
											}										
										}
										ImGui::NextColumn();
										if(ImGui::DragFloat(":y", &curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.y)){
											curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											
											// force follwing filters need_update also
											for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
											{
												curObj->meshFilters[j]->need_update = true;
											}																				
										}
										ImGui::NextColumn();
										if(ImGui::DragFloat(":z", &curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.z)){
											curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
											
											// force follwing filters need_update also
											for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
											{
												curObj->meshFilters[j]->need_update = true;
											}																				
										}
										
										ImGui::Separator();								
										ImGui::Columns(1);	
										
									if(ImGui::DragFloat(
										curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].name.c_str(),
										&curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.x ))
									{
										std::cout << curObj->meshFilters[cur_mesh_filter_selected]->paramsVec3[i].value.x << "\n";
										curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										
										// force follwing filters need_update also
										for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										{
											curObj->meshFilters[j]->need_update = true;
										}									
									}
									
								}														
								
								for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsInt.size(); i++)
								{
									static bool need_update = false;
									
									if(ImGui::DragInt(
										curObj->meshFilters[cur_mesh_filter_selected]->paramsInt[i].name.c_str(),
										&curObj->meshFilters[cur_mesh_filter_selected]->paramsInt[i].value ))
									{
										std::cout << curObj->meshFilters[cur_mesh_filter_selected]->paramsInt[i].value << "\n";
										curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										
										// force follwing filters need_update also
										for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										{
											curObj->meshFilters[j]->need_update = true;
										}										
									}
									
								}

								for (int i = 0; i < curObj->meshFilters[cur_mesh_filter_selected]->paramsBool.size(); i++)
								{
									
									if(ImGui::CheckboxFlags("Invert Normals", (unsigned int*)&curObj->meshFilters[cur_mesh_filter_selected]->paramsBool[i].value, 1))
									{
										curObj->meshFilters[cur_mesh_filter_selected]->need_update = true;
										
										// force follwing filters need_update also
										for (int j = cur_mesh_filter_selected+1; j < curObj->meshFilters.size(); j++)
										{
											curObj->meshFilters[j]->need_update = true;
										}									
									}
								}
								
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

	
	
	
	//~ int ex = explorerDialog();
		
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
	
		for (int i = 0; i < objects.size(); i++)
		{	
		
			Object* curObj = objects[i];
			curObj->shader.useProgram();	
			
				
			//~ glLoadIdentity();
			
			glm::mat4 projection = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 model = glm::mat4(1.0f);
			projection*= glm::perspective(45.0f, (float)width / (float)height, 0.01f, 100.0f);
			 
			 
			// setup modelview matrix 		
			
			
			
			
			
			//~ glm::vec3 eye_pos =	glm::vec3(0.0f, -2.0f, 3.0f);
			//~ glm::vec3 target_pos =	glm::vec3(0.0f,0.0f,0.0f);
			
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
			// model matrix ?? 
			
		
			//~ glm::mat4 ModelMatrix = glm::mat4(1.0f);

			
			model = glm::translate(model, curObj->position);
			
			model = glm::rotate(model, glm::radians(curObj->rotation.x) ,glm::vec3(1.0f,0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(curObj->rotation.y) ,glm::vec3(0.0f,1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(curObj->rotation.z) ,glm::vec3(0.0f,0.0f, 1.0f));
			
			model = glm::scale(model, curObj->scale);

						
						
			// create and upload modelviewprojection matrix				 
			//~ ModelViewProjectionMatrix = projection * ModelViewMatrix * ModelMatrix;		
			
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"projection"), 1, GL_FALSE, glm::value_ptr(projection));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
			glUniformMatrix4fv(glGetUniformLocation(curObj->shader.m_id,"view"), 1, GL_FALSE, glm::value_ptr(view));	
			
			
			
			
			
									
									
			GLuint COLOR_LOC = glGetUniformLocation(curObj->shader.m_id,"u_color");
			glUniform4f(COLOR_LOC, curObj->color.x, curObj->color.y, curObj->color.z, curObj->color.w);
			
			
			//~ glBindTexture(GL_TEXTURE_2D,curObj->texture_id);
			curObj->texture.bind();
			glUniform1i(glGetUniformLocation(curObj->shader.m_id, "u_tex"),0);

			//~ glLoadIdentity();

			//~ glPointSize(5);
			//~ glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			
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
		
	
}

Window::~Window()
{
	
}


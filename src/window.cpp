#include "window.h"

#include <iostream>
#include <typeinfo>
#include <sstream>

#include <array>

Window::Window()
{
	
	
	if(!glfwInit()){
		std::cout<<"Problem with GLFW\n";
		glfwTerminate();
		
	}

	std::cout<<"initializing GLFW\n";
	win = glfwCreateWindow(640,480, "angine", NULL, NULL);


	if(!win){
		std::cout << "Problem with window\n";
		glfwTerminate();
		
	}
	
		
	glfwMakeContextCurrent(win);			
	glewInit();
	
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
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	
	const char* glsl_version = "#version 130";

	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	


}

void Window::addDialog(){
	


	ImGui::Begin("Properties");
	
	static int listbox_item_current = 0;
	
	
	if(ImGui::ListBoxHeader("new List box", 5))
	{
		for (int i = 0; i < objects.size(); i++)
		{
			char text[500];
			sprintf(text, "Object %d", i);
			if(ImGui::Selectable(objects[i]->name, i == listbox_item_current))
			{
				//~ std::cout << "press item " << i << "\n";
				listbox_item_current = i;
			}
			
		}
		
		ImGui::ListBoxFooter();
	}
	
	Object* curObj = objects[listbox_item_current];
	char text[500];
	sprintf(text, "object %d", listbox_item_current);
	if (ImGui::CollapsingHeader(text))
	{				
		
		
		
		
		//~ static char buf[100] = curObj->name;
		ImGui::InputText("name :" , curObj->name, IM_ARRAYSIZE(curObj->name)); 
		sprintf(text, "btn", listbox_item_current);
		if(ImGui::Button(text))
		{
			std::cout << "btn test\n";
		}
		sprintf(text, "Display Bbox", listbox_item_current);
		ImGui::CheckboxFlags(text, (unsigned int*)&objects[listbox_item_current]->bDisplayBoundingBox, 1);

		
		
		//~ float& tx = objects[i]->position.x;
		ImGui::Columns(3,"columns");
		ImGui::InputFloat(":X", &objects[listbox_item_current]->position.x, 0.0f, 1.0f, "%.3f");
		ImGui::NextColumn();
		ImGui::InputFloat(":Y", &objects[listbox_item_current]->position.y, 0.0f, 10.0f, "%.3f");
		ImGui::NextColumn();
		ImGui::InputFloat(":Z", &objects[listbox_item_current]->position.z, 0.0f, 10.0f, "%.3f");
	}
	
	

	ImGui::End();
}

void Window::refresh(){
	

	bool show_demo_window = true;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//~ ImGui::ShowDemoWindow(&show_demo_window);

	addDialog();
	
	ImGui::Render();

	
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0,0,width, height);
	
	float ratio = (float)width / height;
	
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5,0.5,0.5,1.0);

	
	
	
	renderObjects();
	


	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//swap buffers
	glfwSwapBuffers(win);
	
	
	glfwPollEvents();
	
	
	
}

bool Window::shouldClose(){
	return glfwWindowShouldClose(win);
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
			
			model = glm::rotate(model, curObj->rotation.x ,glm::vec3(1.0f,0.0f, 0.0f));
			model = glm::rotate(model, curObj->rotation.y ,glm::vec3(0.0f,1.0f, 0.0f));
			model = glm::rotate(model, curObj->rotation.z ,glm::vec3(0.0f,0.0f, 1.0f));
			
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

			
			glPointSize(5);
			curObj->draw(curObj->getRenderMode());
			
			if(curObj->bDisplayNormals){
											
				COLOR_LOC = glGetUniformLocation(curObj->shader.m_id,"u_color");
				glUniform4f(COLOR_LOC, 1.0,0.0,0.0,1.0);			
				curObj->drawNormals();
			}
			
			if(curObj->bDisplayBoundingBox)
			{
				//~ glUseProgram(0);
				curObj->lineShader.useProgram();
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


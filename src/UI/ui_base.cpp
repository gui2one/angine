#include "ui_base.h"
#include <typeinfo>

UIBase::UIBase()
{

}

void UIBase::init(){

	pane_shader.loadVertexShaderSource("../src/res/shaders/pane_shader.vert");
	pane_shader.loadFragmentShaderSource("../src/res/shaders/pane_shader.frag");	
	pane_shader_id = pane_shader.createShader();	
	
	shader.loadVertexShaderSource("../src/res/shaders/font_shader.vert");
	shader.loadFragmentShaderSource("../src/res/shaders/font_shader.frag");	
	shader_id = shader.createShader();	
	
	stbi_set_flip_vertically_on_load(1);
	font_buffer = stbi_load("../src/res/fonts/samplefont.gif", &fb_width, &fb_height, &bpp, 4);		
	
	
	bitmap_string.init(font_buffer);
	
}

void UIBase::addItem(UIBaseItem* item)
{
	item->font_buffer = font_buffer;
	item->bitmap_string.init(font_buffer);
	items.push_back(item);
	
	std::cout << "adding item of type <" << typeid(*item).name() << "> to BASE UI\n";
}


void UIBase::draw(){
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->draw();
	}
	
}

void UIBase::update()
{
	for (int i = 0; i < items.size(); i++)
	{
		
		if( getCursorPos().x > items[i]->getPosition().x && 
			getCursorPos().x < items[i]->getPosition().x + items[i]->getSize().x)
		{
			//~ std::cout << "Mouse over : "<< items[i]->getLabel() << "\n";
			if( getMouseState().button == 0 && getMouseState().action == GLFW_PRESS){
				items[i]->action();
			}
		}
	}
	
}


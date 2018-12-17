#include "ui_base.h"


UIBase::UIBase()
{

}

void UIBase::init(){
	
	shader.loadVertexShaderSource("../src/res/shaders/font_shader.vert");
	shader.loadFragmentShaderSource("../src/res/shaders/font_shader.frag");
	
	shader_id = shader.createShader();	
	
	stbi_set_flip_vertically_on_load(1);
	font_buffer = stbi_load("../src/res/fonts/samplefont.gif", &fb_width, &fb_height, &bpp, 4);		
	
	
	bitmap_string.init(font_buffer);
	
	//~ for (int i = 0; i < items.size(); i++)
	//~ {
		//~ items[i].bitmap_string.init(font_buffer);
	//~ }
	
}

void UIBase::addItem(UIBaseItem item){
	item.bitmap_string.init(font_buffer);
	items.push_back(item);
}



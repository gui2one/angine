#ifndef UI_BASE_H
#define UI_BASE_H

#include "../pch.h"
#include "../texture.h"
#include "../shader.h"
#include "bitmap_string.h"
#include "ui_base_item.h"
#include "ui_button.h"
class UIBase
{
	public:
		UIBase();
		void init();		
		BitmapString bitmap_string;
		
		std::vector<UIBaseItem> items;
		void addItem(UIBaseItem item);
		
		unsigned char* font_buffer;
		int fb_width, fb_height, bpp;
		
		Shader shader;		
		int shader_id;			
	private:
		/* add your private declarations */
};

#endif /* BASE_UI_H */ 

#ifndef UI_BASE_H
#define UI_BASE_H

#include "../pch.h"
#include "../texture.h"
#include "../shader.h"
#include "bitmap_string.h"
#include "ui_event.h"
#include "ui_container.h"
#include "ui_base_item.h"
#include "ui_button.h"

struct MouseState{
	int button;
	int action;
	int mods;
};

class UIBase
{
	public:
		UIBase();
		void init();		
		BitmapString bitmap_string;
		
		
		std::vector<UIBaseItem*> items;
		void addItem(UIBaseItem* item);
		void draw();
		
		std::vector<UIEvent<MouseEvent>> mouse_events;
		
		unsigned char* font_buffer;
		int fb_width, fb_height, bpp;
		
		inline void setCursorPos(glm::vec2 vec)
		{ 
			cursor_pos = vec; 
			if(old_cursor_pos != cursor_pos){
				old_cursor_pos = cursor_pos;
				//~ printf("-- New cursor pos %.1f %.1f\n", cursor_pos.x, cursor_pos.y);
			}
			update();
		}
		inline glm::vec2 getCursorPos(){ return cursor_pos; }
		
		inline void setMouseState(int button, int action, int mods){
			mouse_state = {button, action, mods};
			update();
			//~ printf("-- New mouse state %d %d %d\n", button, action, mods);
		}
		inline MouseState getMouseState(){
			return mouse_state;
		}
		
		void update();
		
		
		Shader shader;		
		int shader_id;	
		
		Shader pane_shader;
		int pane_shader_id;		
	private:
		MouseState mouse_state;
		glm::vec2 cursor_pos;
		glm::vec2 old_cursor_pos;
		/* add your private declarations */
};

#endif /* BASE_UI_H */ 

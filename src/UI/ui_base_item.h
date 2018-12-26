#ifndef UI_BASE_ITEM_H
#define UI_BASE_ITEM_H

#include "../pch.h"
#include "bitmap_string.h"
class UIBaseItem
{
	public:
		UIBaseItem();
		
		std::vector<UIBaseItem*> children;
		inline void addChild(UIBaseItem* child)
		{
			children.push_back(child);
		}
		
		
		unsigned char* font_buffer;
		
		inline void setLabel(std::string str) { 
			label = str;
			bitmap_string.setValue(str);
		};
		inline std::string getLabel(){ return label; };
		
		inline void setPosition(glm::vec2 vec){ position = vec; }
		inline glm::vec2 getPosition(){ return position; }

		inline void setSize(glm::vec2 vec){ size = vec; }
		inline glm::vec2 getSize(){ return size; };
		
		BitmapString bitmap_string;
		
		virtual void draw();
		virtual void action(){};
	private:
		glm::vec2 position;
		glm::vec2 size;
		std::string label;
	
		/* add your private declarations */
};

#endif /* UI_BASE_ITEM_H */ 

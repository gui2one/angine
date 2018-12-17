#ifndef UI_BASE_ITEM_H
#define UI_BASE_ITEM_H

#include "../pch.h"
#include "bitmap_string.h"
class UIBaseItem
{
	public:
		UIBaseItem();
		std::string label;
		
		void setPosition(float x, float y);
		glm::vec2 getPosition();
		
		BitmapString bitmap_string;
	private:
		glm::vec2 position;
	
		/* add your private declarations */
};

#endif /* UI_BASE_ITEM_H */ 

#ifndef UI_CONTAINER_H
#define UI_CONTAINER_H
//~ #include "../pch.h"
#include "ui_base_item.h"
class UIContainer : public UIBaseItem
{
	public:
		UIContainer();
		//~ UIContainer(unsigned char* _font_buffer);
		
		void draw() override;
			
		//~ void addChild(UIBaseItem* child) override;
		std::vector<UIBaseItem*> children;
		
		unsigned char* font_buffer;
		
	private:
	
		
		/* add your private declarations */
};

#endif /* UI_CONTAINER_H */ 

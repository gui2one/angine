#include "ui_base_item.h"


UIBaseItem::UIBaseItem()
{
	setPosition(glm::vec2(0.0,0.0));
	setSize(glm::vec2(50.0,50.0));
}



void UIBaseItem::draw(){
	//~ std::cout << "TYPE : "<<typeid(this).name() << "\n";
	//~ printf("Base item draw function\n");
}


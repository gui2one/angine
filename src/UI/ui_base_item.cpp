#include "ui_base_item.h"


UIBaseItem::UIBaseItem()
{
	setPosition(0.0,0.0);
}

void UIBaseItem::setPosition(float x, float y){
	position = glm::vec2(x,y);
}

glm::vec2 UIBaseItem::getPosition(){
	return position;
}



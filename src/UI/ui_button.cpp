#include "ui_button.h"

UIButton::UIButton()
{
	setLabel("Press Me");
	//~ std::cout << "Creating Button UI Item\n" ;
}

UIButton::UIButton(std::string _label)
{
	setLabel(_label);
	//~ std::cout << "Label Set to"<<  getLabel() <<"\n" ;
	//~ std::cout << "Creating Button UI Item\n" ;
}


void UIButton::draw()
{
	//~ std::cout << "UIButton Draw() function\n";


	float vert_data[] = {
		0.0, 0.0,   0.0,0.0,
		getSize().x, getSize().y,   1.0,1.0,
		0.0, getSize().y,   0.0,1.0,
		
		0.0, 0.0,  0.0, 0.0,
		getSize().x, 0.0,  1.0, 0.0,
		getSize().x, getSize().y,  1.0, 1.0
	};
	
	glDeleteBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glBufferData(GL_ARRAY_BUFFER , sizeof(float) * 4 * 6, vert_data, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(sizeof(float)*2)); 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
		glDrawArrays(GL_TRIANGLES, 0, 6);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	
	
	
	
	
}

void UIButton::action()
{
	click();
}

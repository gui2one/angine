#ifndef UI_BUTTON_H
#define UI_BUTTON_H
#include "../pch.h"
#include "ui_base_item.h"
class UIButton : public UIBaseItem
{
	public:
		UIButton();
		UIButton(std::string _label);
		
		void draw() override;
		void action() override;
		
		boost::signals2::signal<void()> click;

		
		unsigned int m_vbo;
	private:
		/* add your private declarations */
};

#endif /* UI_BUTTON_H */ 

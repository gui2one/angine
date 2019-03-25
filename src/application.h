#ifndef APPLICATION_H
#define APPLICATION_H



#include "pch.h"
#include "window.h"

#include "imgui_impl_glfw.h"

class Application
{
	public:
		Application();
		virtual ~Application();
		
		Window  window;


	private:
		
		/* add your private declarations */
};

#endif /* APPLICATION_H */ 

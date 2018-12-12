#ifndef APPLICATION_H
#define APPLICATION_H


#include <vector>

#include "window.h"
#include "object.h"
#include "shader.h"
#include "utils/objLoader.h"
class Application
{
	public:
		Application();
		virtual ~Application();
		
		Window  window;

		void printObjectsInfos();
		std::vector<Object*> objects;
	private:
		
		/* add your private declarations */
};

#endif /* APPLICATION_H */ 

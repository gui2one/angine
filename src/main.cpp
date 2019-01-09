#include <iostream>
#include "application.h"
#include <unistd.h>




Application app ;

int main()
{

	std::cout << "angine PROJECT\n";
	
	while(!app.window.shouldClose())
	{		
				
		app.window.refresh();		
	
		//~ usleep(1000000/50);	
	}
	
	return 0;
}

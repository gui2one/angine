#include <iostream>
#include "application.h"
#include <unistd.h>


//~ #include "generators/gridmesh.h"
//~ #include "generators/spheremesh.h"





Application app ;



//~ Mesh loadNewObject(){
	//~ char file[1024];
	//~ FILE *f = popen("zenity --file-selection --title='choose a 3d file' ", "r");
	//~ fgets(file, 1024, f);	
	//~ std::cout << "----- "<<file << "\n";	
	//~ 
	//~ ObjLoader loader;	
	//~ std::string str(file);	
	//~ //// remove end of line 
		//~ str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	//~ 
	//~ 
	//~ Mesh mesh;
	//~ mesh = loader.assimp_load(str);
	//~ 
	//~ return mesh;
//~ }


int main()
{

	std::cout << "angine PROJECT\n";
	
	



	
	
	
	
	while(!app.window.shouldClose())
	{		

				
		app.window.refresh();
		
	
		usleep(1000000/50);
	
	}
	
	return 0;
}

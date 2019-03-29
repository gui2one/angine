#include "handle.h"


Handle::Handle()
{
	printf("Creating handle\n");
	setName("default handle name");
}

TranslateHandle::TranslateHandle():Handle()
{
	//~ printf("Creating handle\n");
	setName("translate_x");
	
}



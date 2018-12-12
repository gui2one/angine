#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GLFW/glfw3.h>
#include <iostream>
class Texture
{
	public:
		
		Texture();
		
		virtual ~Texture();
		void load(std::string path);
		

		inline unsigned int getID(){ 
			std::cout << "return ID is :" << id << "\n";
			return id; 
		};
		void bind();
		void unbind();
		
	private:
		unsigned int id;
		unsigned char* buffer;
		int width, height, bpp;	
	
		/* add your private declarations */
};

#endif /* TEXTURE_H */ 

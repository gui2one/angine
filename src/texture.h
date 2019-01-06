#ifndef TEXTURE_H
#define TEXTURE_H

#include "pch.h"


//~ #include <string>
//~ #include <GLFW/glfw3.h>
#include <iostream>
class Texture
{
	public:
		
		Texture();
		
		virtual ~Texture();
		void load(std::string path);
		void setData(int width, int height, unsigned char* buffer);

		inline unsigned int getID(){ return id; };
		inline int getWidth(){ return width; };
		inline int getHeight(){ return height; };
		inline int getBPP(){ return bpp; };
		void bind();
		void unbind();
		
	private:
		unsigned int id;
		//~ unsigned char* buffer;
		int width, height, bpp;	
	
		/* add your private declarations */
};

#endif /* TEXTURE_H */ 

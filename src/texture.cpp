#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

Texture::Texture():id(0),width(0), height(0), buffer(nullptr), bpp(0)
{
	
	std::cout << "creating texture object... " << "\n";
	

}

void Texture::load(std::string path){
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	
	buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	stbi_set_flip_vertically_on_load(1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	
//~ 
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	if( buffer == nullptr){
		std::cout << "stbi_failure_reason()\n";
		std::cout << stbi_failure_reason() << "\n";
	}
	if(buffer)
		stbi_image_free(buffer);	
}

void Texture::bind(){
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	std::cout << "deleting texture -- ID : " << id << "\n";
	glDeleteTextures(1, &id);
}


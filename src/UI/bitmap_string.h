#ifndef BITMAP_STRING_H
#define BITMAP_STRING_H

#include "../pch.h"
#include "../texture.h"
#include "../shader.h"

struct font_vertex{
	glm::vec2 position;
	glm::vec2 t_coords;
};
struct letter_coords{
	int x,y;
};

struct PixelRect{
	int start_x, start_y, width, height;
};
class BitmapString
{
	public:
		BitmapString();
		void init(unsigned char* _font_buffer);		
		
		void update();
		inline void setValue(std::string str){ m_str_value = str; };
		inline std::string getValue(){ return m_str_value; };
		
		void drawString();	
		void createStringTexture();
		void createStringMesh(float x, float y, int _width, int _height);

		PixelRect charToPixelRect(char ch);
		unsigned char* font_buffer;
		

		GLuint vbo;
		Texture string_map;
		Shader shader;		
		int shader_id;		
		
		
		
			
		
	private:
		std::string m_str_value, m_str_old_value;
		std::vector<font_vertex> vertices;
		/* add your private declarations */
};

#endif /* BITMAP_STRING_H */ 

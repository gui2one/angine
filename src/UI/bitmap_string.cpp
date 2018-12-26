#include "bitmap_string.h"


BitmapString::BitmapString()
{
	m_str_value = "default string";
}

void BitmapString::init(unsigned char* _font_buffer)
{
	font_buffer = _font_buffer;			
	createStringTexture();
}

void BitmapString::update()
{
	if( m_str_old_value != m_str_value)
	{
		createStringTexture();
		m_str_old_value = m_str_value;
		
		//~ std::cout << "Updated bitmap string\n";
	}
}

PixelRect BitmapString::charToPixelRect(char ch)
{
	PixelRect rect;
	int col, row;
	struct temp_coords{
		int col, row, width, height;
	};
	
	temp_coords temp;

	     if(ch == (int)' ') temp = { 0,13,5,16};
	else if(ch == (int)'!') temp = { 1,13,10,16};
	else if(ch == (int)'"') temp = { 2,13,10,16};
	else if(ch == (int)'#') temp = { 3,13,10,16};
	else if(ch == (int)'$') temp = { 4,13,10,16};
	else if(ch == (int)'%') temp = { 5,13,12,16};
	else if(ch == (int)'&') temp = { 6,13,10,16};
	else if(ch == (int)'\'') temp ={ 7,13,10,16};
	else if(ch == (int)'(') temp = { 8,13,10,16};
	else if(ch == (int)')') temp = { 9,13,10,16};
	else if(ch == (int)'*') temp = {10,13,10,16};
	else if(ch == (int)'+') temp = {11,13,10,16};
	else if(ch == (int)',') temp = {12,13,10,16};
	else if(ch == (int)'-') temp = {13,13,10,16};
	else if(ch == (int)'.') temp = {14,13,5,16};
	else if(ch == (int)'/') temp = {15,13,10,16};
	
	else if(ch == (int)'0') temp = { 0,12,10,16};
	else if(ch == (int)'1') temp = { 1,12,10,16};
	else if(ch == (int)'2') temp = { 2,12,10,16};
	else if(ch == (int)'3') temp = { 3,12,10,16};
	else if(ch == (int)'4') temp = { 4,12,10,16};
	else if(ch == (int)'5') temp = { 5,12,10,16};
	else if(ch == (int)'6') temp = { 6,12,10,16};
	else if(ch == (int)'7') temp = { 7,12,10,16};
	else if(ch == (int)'8') temp = { 8,12,10,16};
	else if(ch == (int)'9') temp = { 9,12,10,16};
	else if(ch == (int)':') temp = {10,12,10,16};
	else if(ch == (int)';') temp = {11,12,10,16};
	else if(ch == (int)'<') temp = {12,12,10,16};
	else if(ch == (int)'=') temp = {13,12,10,16};
	else if(ch == (int)'>') temp = {14,12,10,16};
	else if(ch == (int)'?') temp = {15,12,10,16};
	
	else if(ch == (int)'@') temp = { 0,11,13,16};
	else if(ch == (int)'A') temp = { 1,11,10,16};
	else if(ch == (int)'B') temp = { 2,11,10,16};
	else if(ch == (int)'C') temp = { 3,11,10,16};
	else if(ch == (int)'D') temp = { 4,11,10,16};
	else if(ch == (int)'E') temp = { 5,11,10,16};
	else if(ch == (int)'F') temp = { 6,11,10,16};
	else if(ch == (int)'G') temp = { 7,11,10,16};
	else if(ch == (int)'H') temp = { 8,11,10,16};
	else if(ch == (int)'I') temp = { 9,11,6,16};
	else if(ch == (int)'J') temp = {10,11,8,16};
	else if(ch == (int)'K') temp = {11,11,10,16};
	else if(ch == (int)'L') temp = {12,11,10,16};
	else if(ch == (int)'M') temp = {13,11,12,16};
	else if(ch == (int)'N') temp = {14,11,10,16};
	else if(ch == (int)'O') temp = {15,11,10,16};
	else if(ch == (int)'P') temp = { 0,10,10,16};
	else if(ch == (int)'Q') temp = { 1,10,10,16};
	else if(ch == (int)'R') temp = { 2,10,10,16};
	else if(ch == (int)'S') temp = { 3,10,10,16};
	else if(ch == (int)'T') temp = { 4,10,10,16};
	else if(ch == (int)'U') temp = { 5,10,10,16};
	else if(ch == (int)'V') temp = { 6,10,10,16};
	else if(ch == (int)'W') temp = { 7,10,14,16};
	else if(ch == (int)'X') temp = { 8,10,10,16};
	else if(ch == (int)'Y') temp = { 9,10,10,16};
	else if(ch == (int)'Z') temp = {10,10,10,16};
	else if(ch == (int)'[') temp = {11,10,10,16};
	else if(ch == (int)'\\') temp = {12,10,10,16};
	else if(ch == (int)']') temp = {13,10,10,16};
	else if(ch == (int)'^') temp = {14,10,10,16};
	else if(ch == (int)'_') temp = {15,10,10,16};
	
	
	else if(ch == (int)'`') temp = { 0, 9,10,16};
	else if(ch == (int)'a') temp = { 1, 9,10,16};
	else if(ch == (int)'b') temp = { 2, 9,10,16};
	else if(ch == (int)'c') temp = { 3, 9,10,16};
	else if(ch == (int)'d') temp = { 4, 9,10,16};
	else if(ch == (int)'e') temp = { 5, 9,10,16};
	else if(ch == (int)'f') temp = { 6, 9, 6,16};
	else if(ch == (int)'g') temp = { 7, 9,10,16};
	else if(ch == (int)'h') temp = { 8, 9,10,16};
	else if(ch == (int)'i') temp = { 9, 9, 6,16};
	else if(ch == (int)'j') temp = {10, 9, 7,16};
	else if(ch == (int)'k') temp = {11, 9,10,16};
	else if(ch == (int)'l') temp = {12, 9, 5,16};
	else if(ch == (int)'m') temp = {13, 9,12,16};
	else if(ch == (int)'n') temp = {14, 9, 7,16};
	else if(ch == (int)'o') temp = {15, 9,10,16};
	else if(ch == (int)'p') temp = { 0, 8, 9,16};
	else if(ch == (int)'q') temp = { 1, 8, 9,16};
	else if(ch == (int)'r') temp = { 2, 8, 6,16};
	else if(ch == (int)'s') temp = { 3, 8,10,16};
	else if(ch == (int)'t') temp = { 4, 8, 6,16};
	else if(ch == (int)'u') temp = { 5, 8,10,16};
	else if(ch == (int)'v') temp = { 6, 8,10,16};
	else if(ch == (int)'w') temp = { 7, 8,12,16};
	else if(ch == (int)'x') temp = { 8, 8,10,16};
	else if(ch == (int)'y') temp = { 9, 8,10,16};
	else if(ch == (int)'z') temp = {10, 8,10,16};


	else if(ch == (int)'{') temp = {11, 8,10,16};
	else if(ch == (int)'|') temp = {12, 8,10,16};
	else if(ch == (int)'}') temp = {13, 8,10,16};
	else if(ch == (int)'~') temp = {14, 8,10,16};
	
	
	else temp = {0,0,12,16};
	
	 
	 
	 rect.start_x = temp.col * (224/16);
	 rect.start_y = temp.row * (224/14);
	 rect.width = temp.width;
	 rect.height = temp.height;
	 
	return rect;
}

void BitmapString::createStringTexture()
{	
	std::string test_str;
	test_str = m_str_value;
	
	std::vector<PixelRect> rects;
	int inc_width = 0;
	for (int i = 0; i < m_str_value.size(); i++)
	{
		PixelRect rect;
		rect = charToPixelRect(test_str[i]);
		rects.push_back(rect);
		inc_width += rect.width;
	}
	

	

	const int copy_width = inc_width;
	const int copy_height = 16;	
	unsigned char copy_data[copy_width * copy_height * 4];
					
	string_map.setData(copy_width, copy_height, copy_data);

	string_map.bind();	
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	
	
	int offset_x = 0;
	int offset_y = 0;
	
	for (int c = 0; c < rects.size(); c++)
	{
		
		PixelRect rect;
		rect = rects[c];		
	
		// read some pixels from the font_buffer
		
		const int read_data_width = rect.width;
		const int read_data_height = rect.height;	
		
		unsigned char read_data[read_data_width * read_data_height * 4];
		
		for (size_t i = 0; i < read_data_width*read_data_height; i++)
		{
			size_t start_x = rect.start_x;
			size_t start_y = rect.start_y;

			size_t fb_x, fb_y;

			fb_x = ((i % read_data_width) + start_x);
			fb_y = ((i / read_data_width) + start_y);			

			read_data[(i*4)+0] = font_buffer[(fb_x +(fb_y * 224) ) * 4];
			read_data[(i*4)+1] = 0;
			read_data[(i*4)+2] = 0;
			read_data[(i*4)+3] = 255;
		}
		
		//~ std::cout << "--- Final texture id :" << ft_id << "\n";		
		//~ std::cout << "--- width :" << fb_width << "\n";
		//~ std::cout << "--- height :" << fb_height << "\n";	

		glTexSubImage2D(GL_TEXTURE_2D, 0, offset_x, offset_y,read_data_width,read_data_height, GL_RGBA,GL_UNSIGNED_BYTE, &read_data[0]);
		offset_x += rect.width;
		
		
	}
	
	
	rects.clear();
	
	string_map.unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
	
	vertices.clear();
	createStringMesh(0.0,0.0 , copy_width, 16);	
	
	
	glDeleteBuffers(1, &vbo);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * vertices.size(), vertices.data(), GL_STATIC_DRAW); 	
	
	glBindBuffer(GL_ARRAY_BUFFER,0);	
}

void BitmapString::createStringMesh(float x, float y, int _width, int _height)
{
	float scale = 1.0;

	float texture_width = (float)_width;
	float texture_height = (float)_height;
	font_vertex f_vtx;
	f_vtx.position = glm::vec2((0.0 + x) * scale * texture_width,(0.0 + y)* scale * texture_height);
	f_vtx.t_coords = glm::vec2(0.0, 0.0);
	vertices.push_back(f_vtx);
	
	f_vtx.position = glm::vec2((1.0+x)* scale * texture_width, (1.0+y)* scale  * texture_height);
	f_vtx.t_coords = glm::vec2(1.0, 1.0);
	vertices.push_back(f_vtx);
	
	f_vtx.position = glm::vec2(x * scale * texture_width , (1.0 + y)* scale  * texture_height);
	f_vtx.t_coords = glm::vec2(0.0, 1.0);
	vertices.push_back(f_vtx);	
	
	f_vtx.position = glm::vec2(x * scale * texture_width,y * scale  * texture_height);
	f_vtx.t_coords = glm::vec2(0.0, 0.0);
	vertices.push_back(f_vtx);	
	
	f_vtx.position = glm::vec2((1.0 + x)* scale * texture_width,y * scale  * texture_height);
	f_vtx.t_coords = glm::vec2(1.0, 0.0);
	vertices.push_back(f_vtx);		
	
	f_vtx.position = glm::vec2((1.0 + x) * scale * texture_width, (1.0 + y) * scale  * texture_height);
	f_vtx.t_coords = glm::vec2(1.0, 1.0);
	vertices.push_back(f_vtx);			
}

void BitmapString::drawString()
{
	string_map.bind();
	glUniform1i(glGetUniformLocation(shader.m_id, "u_tex"),0);	

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0); 			
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (const void*)(2 * sizeof(float))); 			
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0,vertices.size());

	//// unbind everything
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);	
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	string_map.unbind();
	glUseProgram(0);
}


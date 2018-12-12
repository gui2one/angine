#include "shader.h"

Shader::Shader(){
	
}

Shader::~Shader(){
	
}

std::string loadShaderFile(std::string path){

        std::ifstream input(path);
        std::string output = "";
        for( std::string line; std::getline(input, line);){
                        //~ std::cout << line << std::endl;
                        output += line;
                        output += "\n";
        }

        return output;

}



void Shader::loadVertexShaderSource(std::string filePath){
	vertexShaderSource = loadShaderFile(filePath);
}
void Shader::loadFragmentShaderSource(std::string filePath){
	fragmentShaderSource = loadShaderFile(filePath);
}

std::string Shader::getVertexShaderSource(){
	return vertexShaderSource;
}
std::string Shader::getFragmentShaderSource(){
	return fragmentShaderSource;
}





unsigned int Shader::compileShader(unsigned int type, const std::string& source){
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);

        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if( result == GL_FALSE ){
                int length;
                glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)alloca(length * sizeof(char));
                glGetShaderInfoLog(id, length, &length, message);
                std::cout << "Shader compile failure !!!!!"  << std::endl;
                std::cout << "in " << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader") << std::endl;
                std::cout << message << std::endl;

        }

        return id;
}

unsigned int Shader::createShader( const std::string& vertexShader, const std::string& fragmentShader){
        unsigned int program = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);

        glLinkProgram(program);
        glValidateProgram(program);


        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
}

unsigned int Shader::createShader(){
        m_id = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);

        glLinkProgram(m_id);
        glValidateProgram(m_id);


        glDeleteShader(vs);
        glDeleteShader(fs);

        return m_id;
}


void Shader::useProgram(){
	glUseProgram(m_id);
}

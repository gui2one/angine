#ifndef PCH_H
#define PCH_H

#include <string>
#include <vector>
#include <iostream>
//~ #include <boost/signals2.hpp>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
//~ 
//~ #include "vendor/imgui/imgui.h"
//~ #include "vendor/imgui/imgui_impl_glfw.h"
//~ #include "vendor/imgui/imgui_impl_opengl3.h"





#include "vendor/stb_image.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtx/matrix_decompose.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"

#define PI 3.14159265359

#include "vendor/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;
////


// opengl debugging

#include <csignal>

#define ASSERT(x) if(!(x)) std::raise(SIGINT);
#define GLCall(x) GLClearError(); \
	x;\
	ASSERT(GLLogCall())


static void GLClearError()
{
	while(glGetError() != GL_NO_ERROR);
}

static bool GLLogCall()
{
	while( GLenum error = glGetError()){
		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
		return false;
	}
	
	return true;
}

#endif


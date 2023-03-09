#pragma once

// Shader class

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif defined(WIN32)
#define NOMINMAX
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif defined(__LINUX__)
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include <string>
#include <map>

class BJE_Shader
{
public:

	BJE_Shader();
	~BJE_Shader();

	

	GLuint get_program(std::string const& prog_name);

private:
	// Compile program
	GLuint compile_program(std::string const& prog_name);

	BJE_Shader(BJE_Shader const&);
	BJE_Shader& operator=(BJE_Shader const&);

	std::map<std::string, GLuint> programs_;

};













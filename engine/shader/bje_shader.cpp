// Author: Theo Baudoin

#include "bje_shader.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>

// Load file content
static std::string load_file(std::string const& name, std::vector<char>& contents, bool binary = false)
{
	std::ifstream file(name, std::ios::in | (binary ? std::ios::binary : (std::ios_base::openmode)0));

	if (file && file.is_open() && !file.fail())
	{
		contents.clear();

		std::streamoff begin = file.tellg();
		file.seekg(0, std::ios::end);

		std::streamoff file_size = file.tellg() - begin;
		file.seekg(0, std::ios::beg);

		contents.resize((size_t)file_size);
		file.read(contents.data(), file_size);
	}
	else
	{
		std::cout << "Error: Could not open file " << name << std::endl;
		exit(-1);
	}
}

// Compile shader
static GLuint compile_shader(GLenum type, std::string const& source)
{
	GLuint shader = glCreateShader(type);
	char const* src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetShaderInfoLog(shader, log_length, &log_length, log.data());

		std::string const shader_type = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		std::cout << "Error: Failed to compile " << shader_type << " shader:" << std::endl;
		std::cout << log.data() << std::endl;

		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

// Constructor
BJE_Shader::BJE_Shader()
{
	
}


BJE_Shader::~BJE_Shader()
{
	for (auto shader = programs_.cbegin(); shader != programs_.cend(); ++shader)
	{
		glDeleteProgram(shader->second);
	}
}

// Compile program
GLuint BJE_Shader::compile_program(std::string const& prog_name)
{
	std::string vertex_shader_name = prog_name + ".vert";
	std::string fragment_shader_name = prog_name + ".frag";

	// Wrap the shader source code in a string
	std::vector<char> source_code;

	// Wrap the shader source code in a string
	load_file(vertex_shader_name, source_code);
	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, source_code.data());

	// Wrap the shader source code in a string
	load_file(fragment_shader_name, source_code);
	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, source_code.data());

	// Create the program
	GLuint program = glCreateProgram();

	// Attach the shaders to the program
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	// Delete the shaders
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	glLinkProgram(program);

	// Check the program
	GLint result = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		GLint log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

		std::vector<GLchar> log(log_length);
		glGetProgramInfoLog(program, log_length, &log_length, log.data());

		std::cout << "Error: Failed to link program:" << std::endl;
		std::cout << log.data() << std::endl;

		glDeleteProgram(program);
		exit(-1);
	}

	return program;
}

// Get program
GLuint BJE_Shader::get_program(std::string const& prog_name)
{
	// Find the program in the map
	auto program = programs_.find(prog_name);

	// If the program is not in the map, compile it
	if (program == programs_.end())
	{
		// Compile the program
		program = programs_.insert(std::make_pair(prog_name, compile_program(prog_name))).first;
	}

	// Return the program
	return program->second;
}








































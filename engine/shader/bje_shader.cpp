// Author: Theo Baudoin

#include "bje_shader.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <iostream>

// Load file content
static void load_file(std::string const& name, std::vector<char>& contents, bool binary = false)
{
	std::ifstream file(name, std::ios::in | (binary ? std::ios::binary : (std::ios_base::openmode)0));

	if (file && file.is_open() && !file.fail())
	{
		contents.clear();

		std::streamoff begin = file.tellg();
		file.seekg(0, std::ios::end);

		std::streamoff file_size = file.tellg() - begin;
		file.seekg(0, std::ios::beg);

		contents.resize(static_cast<unsigned>(file_size));
		file.read(contents.data(), file_size);
	}
	else
	{
		std::cout << "Error: Could not open file " << name << std::endl;
		exit(-1);
	}
}

// Compile shader
static GLuint compile_shader(GLenum type, std::vector<GLchar> const& source)
{
	GLuint shader = glCreateShader(type);

	GLint len = static_cast<GLint>(source.size());
	GLchar const* source_array = &source[0];

	glShaderSource(shader, 1, &source_array, &len);
	glCompileShader(shader);

	GLint result = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		std::vector<char> log;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		log.resize(len);

		glGetShaderInfoLog(shader, len, &result, &log[0]);

		glDeleteShader(shader);

		throw std::runtime_error(std::string(log.begin(), log.end()));

		return 0;
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
	std::string vsName = prog_name + ".vert";
	std::string fsName = prog_name + ".frag";

	// Need to wrap the shader program here to be exception-safe
	std::vector<GLchar> source_code;


	load_file(vsName, source_code);
	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, source_code);


	load_file(fsName, source_code);
	GLuint frag_shader = compile_shader(GL_FRAGMENT_SHADER, source_code);

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, frag_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(frag_shader);

	glLinkProgram(program);

	GLint result = GL_TRUE;
	glGetProgramiv(program, GL_LINK_STATUS, &result);

	if (result == GL_FALSE)
	{
		GLint length = 0;
		std::vector<char> log;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

		log.resize(length);

		glGetProgramInfoLog(program, length, &result, &log[0]);

		glDeleteProgram(program);

		throw std::runtime_error(std::string(log.begin(), log.end()));
	}

	return program;
}

// Get program
GLuint BJE_Shader::get_program(std::string const& prog_name)
{
	auto iter = programs_.find(prog_name);

	if (iter != programs_.end())
	{
		return iter->second;
	}
	else
	{
		GLuint program = compile_program(prog_name);
		programs_[prog_name] = program;
		return program;
	}
}








































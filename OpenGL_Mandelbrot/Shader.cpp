#include <Shader.h>

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <iostream>


/*
Constructor for the shader class. Takes as inputs file paths to the vertex shader code and
to the fragment shader code, respectively. The constructor reads the programs from the given paths,
compiles the shaders, creates a shader program, and links the vertex and fragment shaders to this
program. The ID data member is set to the ID of the shader program.
@param vertexPath The file path to the vertex shader code.
@param fragmentPath The file path to the fragmnet shader code.
*/
Shader::Shader(const char* vs_source, const char* fs_source)
{

	//--------------------------------------------------------------------
	//compile shaders from code
	//--------------------------------------------------------------------

	//create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//attach vertex source to shader
	glShaderSource(vertexShader, 1, &vs_source, NULL);

	//compile shader
	glCompileShader(vertexShader);

	//check for compilation errors
	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
	}


	//create fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//attach fragment source to shader
	glShaderSource(fragmentShader, 1, &fs_source, NULL);

	//compile shader
	glCompileShader(fragmentShader);

	//check for compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl;
	}

	//--------------------------------------------------------------------
	//create shader program and link shaders
	//--------------------------------------------------------------------

	//create the shader program and store the ID as the ID member variable
	ID = glCreateProgram();

	//attach the shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	//link the shaders
	glLinkProgram(ID);

	//check for linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl;
	}

	//delete shader objects now that we are done with them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

/*
Activates this class' associated shader program in OpenGL.
*/
void Shader::use()
{
	glUseProgram(ID);
}

/*
Sets the value of the uniform with the given name to the given boolean value.
@param name The name of the shader uniform being set.
@param value The boolean value to which the uniform will be set.
*/
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

/*
Sets the value of the uniform with the given name to the given integer value.
@param name The name of the shader uniform being set.
@param value The integer value to which the uniform will be set.
*/
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
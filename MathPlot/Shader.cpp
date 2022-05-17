#include "Shader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

Shader* currentShader;

Shader::Shader(const char* vertexFilePath, const char* fragmentFilePath)
{
	//create the shaders
	const auto vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	const auto fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//read the shader code
	std::string vertexShaderCode, fragmentShaderCode;
	readShaderCode(vertexShaderCode, vertexFilePath);
	readShaderCode(fragmentShaderCode, fragmentFilePath);

	//compile the shader
	compileShader(vertexShaderID, vertexFilePath, vertexShaderCode);
	compileShader(fragmentShaderID, fragmentFilePath, fragmentShaderCode);

	//link the shaders
	const auto programID = glCreateProgram();
	linkShader(programID, vertexShaderID, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	id = programID;
}

Shader::~Shader()
{
	//delete shader program
	glDeleteProgram(this->id);
}

void Shader::use()
{
	//bind the shader
	glUseProgram(id);

	//update current shader
	currentShader = this;
}

Shader::operator GLuint() const
{
	return id;
}

void Shader::readShaderCode(std::string &shaderCode, const std::string &filePath) const
{

	std::ifstream shaderStream(filePath, std::ios::in);
	if (shaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(shaderStream, line))
			shaderCode += "\n" + line;
	}
	else
	{
		std::cerr << "Shader file " + filePath + " cannot be open" << std::endl;
	}
	
	shaderStream.close();
}

void Shader::compileShader(const GLuint &shaderID, const std::string &filePath, const std::string &shaderCode)
{
	//compile the shader
	char const* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, nullptr);
	glCompileShader(shaderID);

	//check the shader
	GLint result, infoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<GLchar> shaderErrorMessage(infoLogLength + 1);//provide at least one space to prevent access violent
	glGetShaderInfoLog(shaderID, infoLogLength, nullptr, &shaderErrorMessage[0]);
	if (shaderErrorMessage[0])
	{
		std::cerr << "Compile shader source:" + static_cast<std::string>(&shaderErrorMessage[0]) << std::endl;
	}
}

void Shader::linkShader(const GLuint &programID, const GLuint &vertexShaderID, const GLuint &fragmentShaderID)
{
	//link the program
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	//check the program
	GLint result, infoLogLength;
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<GLchar> programErrorMessage(infoLogLength > 1 ? infoLogLength : 1);
	glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
	if (programErrorMessage[0])
	{
		std::cerr << ("Linking shader program: " + static_cast<std::string>(&programErrorMessage[0])) << std::endl;;
	}
}

GLuint getUniformLocation(const std::string& name)
{
	static UniformBuffer::iterator i;
	i = currentShader->locationBuffer.find(name);
	if (i == currentShader->locationBuffer.end())
	{
		currentShader->locationBuffer[name] = glGetUniformLocation(*currentShader, name.c_str());
		return currentShader->locationBuffer[name];
	}
	else
	{
		return i->second;
	}
}

void updateViewMatrix(const glm::vec3 &viewPosition, const glm::mat4 & viewMatrix)
{
	glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3fv(getUniformLocation("viewPos"), 1, &viewPosition[0]);
}

void updateModelMatrix(const glm::mat4 & modelMatrix)
{
	glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void updateModelMatrix(const glm::vec3 & position)
{
	updateModelMatrix(glm::translate(glm::mat4(), position));
}

void updateProjectionMatrix(const double & fovy, const double & aspect, const double & near, const double &far)
{
	glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::perspective(fovy, aspect, near, far))));
}
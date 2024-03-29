#pragma once
#include <map>
#include <string>

#include <glad/glad.h>

#include <glm\glm.hpp>

//buffer the location of uniforms
typedef std::map<std::string, GLuint> UniformBuffer;

class Shader
{
public:
	//constructor
	Shader(const char* vertexFilePath, const char* fragmentFilePath);

	//destructor
	~Shader();

	void use();
	friend GLuint getUniformLocation(const std::string& name);
	operator GLuint() const;
private:
	GLuint id;
	UniformBuffer locationBuffer;


	//read-shader functions
	void readShaderCode(std::string & shaderCode, const std::string & filePath) const;
	void compileShader(const GLuint & shaderID, const std::string & filePath, const std::string & shaderCode);
	void linkShader(const GLuint & programID, const GLuint & vertexShaderID, const GLuint & fragmentShaderID);
};

GLuint getUniformLocation(const std::string& name);

void updateViewMatrix(const glm::vec3 &viewPosition, const glm::mat4 & viewMatrix);

void updateModelMatrix(const glm::mat4 & modelMatrix);

void updateModelMatrix(const glm::vec3 &position);

void updateProjectionMatrix(const double & fovy, const double & aspect, const double & near, const double &far);
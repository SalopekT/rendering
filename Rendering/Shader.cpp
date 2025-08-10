
#include "Shader.hpp"
#include <glad/glad.h>

Shader::Shader(std::string pathName) : pathName(pathName), id(0) {}

unsigned int VertexShader::createShader() {
	this->id = glCreateShader(GL_VERTEX_SHADER);
}


unsigned int FragmentShader::createShader() {
	this->id = glCreateShader(GL_FRAGMENT_SHADER);
}


void Shader::compileShader() {

}
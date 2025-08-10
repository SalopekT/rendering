
#include "Shader.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


Shader::Shader(std::string pathName) : pathName(pathName), id(0) {}
Shader::~Shader() {
    glDeleteShader(this->id);
}

unsigned int Shader::getId() {
    return this->id;
}

unsigned int VertexShader::createShader() {
	this->id = glCreateShader(GL_VERTEX_SHADER);
    return this->id;
}


unsigned int FragmentShader::createShader() {
	this->id = glCreateShader(GL_FRAGMENT_SHADER);
    return this->id;
}


void Shader::compileShader() {
    //reading shader from source code
    std::ifstream file(this->pathName);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string shaderSrcCode = buffer.str();
    const char* shaderSrcCodePtr = shaderSrcCode.c_str();

    //compiling a shader
    glShaderSource(this->id, 1, &shaderSrcCodePtr, NULL);
    glCompileShader(this->id);

    //checking if compiling succeded
    int  success;
    char infoLog[512];
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}


ShaderProgramme::ShaderProgramme(Shader* vs, Shader* fs) : vs(vs), fs(fs) {
    //creating a shader programme
    this->id = glCreateProgram();

    //linking shaders
    glAttachShader(this->id, vs->getId());
    glAttachShader(this->id, fs->getId());
    glLinkProgram(this->id);

    glUseProgram(this->id);
}

void ShaderProgramme::setUniforms(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    //setting uniforms
    int viewMat = glGetUniformLocation(this->id, "view");
    int projectionMat = glGetUniformLocation(this->id, "projection");
    int modelMat = glGetUniformLocation(this->id, "model");

    glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMat, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));


}

void ShaderProgramme::checkLinkingSuccess() {
    int success;
    char infoLog[512];
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING FAILED\n" << infoLog << std::endl;
    }
}

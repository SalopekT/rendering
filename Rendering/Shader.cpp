
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
    //std::cout << shaderSrcCode << std::endl;

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

void ShaderProgramme::setUniformsVertexShader(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    //setting uniforms
    int viewMat = glGetUniformLocation(this->id, "view");
    int projectionMat = glGetUniformLocation(this->id, "projection");
    int modelMat = glGetUniformLocation(this->id, "model");

    glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMat, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(modelMatrix));


}

void ShaderProgramme::setUniformsFragmentShader(int numLights, glm::vec3* lightPosition, glm::mat3* lightIntensities, glm::vec3 materialLightCoefs, glm::vec3 eyePosition, bool hasTexture) {
    //setting uniforms
    int numLightsLoc = glGetUniformLocation(this->id, "numLights");
    int lightPosLoc = glGetUniformLocation(this->id, "lightPositions");
    int lightIntensitiesLoc = glGetUniformLocation(this->id, "lightIntensities");
    int materialLightCoefsLoc = glGetUniformLocation(this->id, "materialLightCoefs");
    int eyePosLoc = glGetUniformLocation(this->id, "eyePosition");
    int textureLoc = glGetUniformLocation(this->id, "texture1");

    glUniform1i(numLightsLoc, numLights);
    glUniform3fv(lightPosLoc, numLights, glm::value_ptr(lightPosition[0]));
    glUniformMatrix3fv(lightIntensitiesLoc, numLights, GL_FALSE, glm::value_ptr(lightIntensities[0]));
    glUniform3f(materialLightCoefsLoc, materialLightCoefs.x, materialLightCoefs.y, materialLightCoefs.z);
    glUniform3f(eyePosLoc, eyePosition.x, eyePosition.y, eyePosition.z);

    if (hasTexture) {
        glUniform1i(textureLoc, 0);
        glUniform1i(glGetUniformLocation(this->id, "hasTexture"), 1);
    }
    else {
        glUniform1i(glGetUniformLocation(this->id, "hasTexture"), 0);
    }

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

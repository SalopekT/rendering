#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Shader {
	protected:
		unsigned int id;
		std::string pathName;

	public:
		Shader(std::string pathName);
		~Shader();

		virtual unsigned int createShader() = 0;
		void compileShader();
		unsigned int getId();

};


class VertexShader : public Shader {
	public:
		VertexShader(const std::string& path) : Shader(path) {}
		unsigned int createShader() override;
};

class FragmentShader : public Shader {
	public:
		FragmentShader(const std::string& path) : Shader(path) {}
		unsigned int createShader() override;
};


class ShaderProgramme {
	private:
		unsigned int id;

		Shader* vs;
		Shader* fs;
		std::vector<Shader*> otherShaders;

	public:
		ShaderProgramme(Shader* vs, Shader* fs);
		void setUniformsVertexShader(glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
		void setUniformsFragmentShader(int numLights, int* typeLights, glm::vec3* lightPosition, glm::mat3* lightIntensities, 
										 glm::vec3* lightDirections, float* lightCutoffAngles,glm::vec3 eyePosition);
		void setObjectUniformsFragmentShader(glm::vec3 materialLightCoefs, bool hasTexture);
		void setUniformsShadowShader(glm::mat4 lightTransformationMatrix, glm::mat4 modelMatrix);
		void checkLinkingSuccess();
		void useProgramme();
		unsigned int getId();
		~ShaderProgramme();





};

#endif
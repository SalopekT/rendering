#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

class Shader {
	protected:
		unsigned int id;
		std::string pathName;

	public:
		Shader(std::string pathName);
		virtual unsigned int createShader() = 0;
		void compileShader();

};


class VertexShader : public Shader {
	public:
		unsigned int createShader() override;
};

class FragmentShader : public Shader {
	public:
		unsigned int createShader() override;
};

#endif
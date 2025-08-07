#ifndef RENDERER_HPP
#define RENDERER_HPP

#include<vector>
#include "Object.hpp"
class Renderer {
	private:
		std::vector<Object> objects;

    public:
		void generateVBO();

};

#endif
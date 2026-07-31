#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 Normal; //normals are needed in fragment shader because of lightning calculations
out vec3 FragPos; //Frag position is position in world coordinates (view and projection transformations not applied)
out vec2 TexCoord;
out vec3 FragPositionLightSpace;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0f));
	Normal = normal; //this Normal value is interpolated so that Phong shading works
	TexCoord = aTexCoord; //just pass uvss to fragment shader
}
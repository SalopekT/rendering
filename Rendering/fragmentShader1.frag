#version 460 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

//https://community.khronos.org/t/adding-normals-into-fragment-shader/73506

uniform vec3 lightPosition;
uniform mat3 lightIntensities; //first column ambient(r,g,b), second diffuse(r,g,b), third specular(r,g,b)
uniform vec3 materialLightCoefs;
    
void main()
{
    vec3 ambientLightInts = lightIntensities[0];
    vec3 ambientColor = materialLightCoefs[0]*ambientLightInts;

    vec3 vectorToLight = lightPosition - FragPos;

    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 
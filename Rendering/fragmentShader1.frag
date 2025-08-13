#version 460 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

//https://community.khronos.org/t/adding-normals-into-fragment-shader/73506

uniform vec3 lightPosition;
uniform mat3 lightIntensities; //first column ambient(r,g,b), second diffuse(r,g,b), third specular(r,g,b)
uniform vec3 materialLightCoefs;
uniform vec3 eyePosition;
    
void main()
{
    //ambient calculations
    vec3 ambientLightInts = lightIntensities[0];
    vec3 ambientColor = materialLightCoefs[0]*ambientLightInts;

    //diffuse calculations
    vec3 vectorToLight = normalize(lightPosition - FragPos);
    float scalarProductDiffuse = max(0,dot(normalize(Normal), vectorToLight));
    vec3 diffuseLightInts = lightIntensities[1];
    vec3 diffuseColor = materialLightCoefs[1]*diffuseLightInts*scalarProductDiffuse;

    //specular calculations
    vec3 vectorToEye = normalize(eyePosition - FragPos);
    vec3 halfVector = (vectorToLight + vectorToEye)/2; // this is an approximation of a reflected ray from light to FragPos to Eye
    float scalarProductSpecular = max(0, pow(dot(halfVector, normalize(Normal)),32.0f));
    vec3 specularLightInts = lightIntensities[2];
    vec3 specularColor = materialLightCoefs[2]*specularLightInts*scalarProductSpecular;

    vec3 color = ambientColor + diffuseColor + specularColor;
    color = clamp(color, 0.0, 1.0);


    FragColor = vec4(color, 1.0f);
} 
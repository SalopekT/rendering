#version 460 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

//https://community.khronos.org/t/adding-normals-into-fragment-shader/73506

uniform int numLights;
uniform vec3 lightPositions[2];
uniform mat3 lightIntensities[2]; //each value in array is a light source, first column ambient(r,g,b), second diffuse(r,g,b), third specular(r,g,b)
uniform vec3 materialLightCoefs;
uniform vec3 eyePosition;
uniform sampler2D texture1;

uniform bool hasTexture;
    
void main()
{
    //ambient calculations
    vec3 ambientLightInts = lightIntensities[0][0]; //ambient component only taken from 1st light src
    vec3 ambientColor = materialLightCoefs[0]*ambientLightInts;

    //diffuse calculations
    vec3 totalDiffuse = vec3(0.0);
    for (int i=0;i<numLights;i++){
        vec3 vectorToLight = normalize(lightPositions[i] - FragPos);
        float scalarProductDiffuse = max(0,dot(normalize(Normal), vectorToLight));
        vec3 diffuseLightInts = lightIntensities[i][1];
        vec3 diffuseColor = materialLightCoefs[1]*diffuseLightInts*scalarProductDiffuse;
        totalDiffuse+=diffuseColor;
    } 
    //specular calculations
    vec3 totalSpecular = vec3(0.0);
        for (int i=0;i<numLights;i++){
        vec3 vectorToEye = normalize(eyePosition - FragPos);
        vec3 vectorToLight = normalize(lightPositions[i] - FragPos);
        vec3 halfVector = (vectorToLight + vectorToEye)/2; // this is an approximation of a reflected ray from light to FragPos to Eye
        float scalarProductSpecular = max(0, pow(dot(halfVector, normalize(Normal)),32.0f));
        vec3 specularLightInts = lightIntensities[i][2];
        vec3 specularColor = materialLightCoefs[2]*specularLightInts*scalarProductSpecular;
        totalSpecular+=specularColor;
    } 
    vec3 color = ambientColor + totalDiffuse + totalSpecular;
    color = clamp(color, 0.0, 1.0);
    vec3 baseColor = texture(texture1, TexCoord).rgb;

    if (hasTexture) FragColor = vec4(baseColor*color, 1.0f);
    else  FragColor = vec4(color, 1.0f);

} 
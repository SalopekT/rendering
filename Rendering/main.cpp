#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Object.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Lightning.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"




void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera* cam);

int width = 1200;
int height = 1000;


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Programme", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, width, height);

    //backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //depth test enabling
    glEnable(GL_DEPTH_TEST);

    //camera settings
    glm::vec3 camPosition(10, 10, 5);
    glm::vec3 camCenter(0, 0, 0);
    glm::vec3 camViewUp(0, 1, 0);
    Camera* cam = new PerspectiveCamera(camPosition, camCenter, camViewUp, 45.0f, width, height, 0.01f, 100.0f);

    //light settings
    glm::vec3 lightPosition(10, 10, 10);
    glm::vec3 lightAmbient(0.6f, 0.6f, 0.6f);
    glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 lightSpecular(0.5f, 0.5f, 0.5f);
    Lightning* lightSrc = new Lightning(lightPosition, lightAmbient, lightDiffuse, lightSpecular);


    //object creation
    Object* obj = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\boombox_4k.fbx\\boombox_4k.fbx");
    //Object* obj = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\dragon.obj");
    //obj->print();

    //generating a texture
    int width1, height1, nrChannels;
    unsigned char* data = stbi_load("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\boombox_4k.fbx\\textures\\boombox_diff_4k.jpg", &width1, &height1, &nrChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    //selecting a texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    unsigned int VAOid = obj->getMesh(0).createBuffer(); //VAO already knows about its VBO


    Shader* vertexShader = new VertexShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\vertexShader1.vert");
    vertexShader->createShader();
    vertexShader->compileShader();

    Shader* fragmentShader = new FragmentShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\fragmentShader1.frag");
    fragmentShader->createShader();
    fragmentShader->compileShader();


    ShaderProgramme* shaderProgramme = new ShaderProgramme(vertexShader, fragmentShader);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(8.0f));
    shaderProgramme->setUniformsVertexShader(model, cam->getLookAtMatrix(), cam->getProjectionMatrix());
    glm::vec3 material(0.8f, 0.8f, 0.3f);
    shaderProgramme->setUniformsFragmentShader(lightSrc->getLightPosition(), lightSrc->getIntensitiesMatrix(), material, cam->getCameraPosition(),true);
    shaderProgramme->checkLinkingSuccess();

    //render loop with double buffering
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, cam);

        //changing uniforms if needed
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(8.0f));

        shaderProgramme->setUniformsVertexShader(model, cam->getLookAtMatrix(), cam->getProjectionMatrix());
        shaderProgramme->setUniformsFragmentShader(lightSrc->getLightPosition(), lightSrc->getIntensitiesMatrix(), material, cam->getCameraPosition(),true);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, obj->getMesh(0).getNumOfVertices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete obj;
    delete cam;
    delete lightSrc;
    delete vertexShader;
    delete fragmentShader;
    delete shaderProgramme;
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera* cam)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cam->moveCamera(1);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cam->moveCamera(2);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cam->moveCamera(3);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cam->moveCamera(4);
    }
}
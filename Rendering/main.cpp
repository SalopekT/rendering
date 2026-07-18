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

    //light settings, multiple lights
    glm::vec3 light1Position(10, 10, 10);
    glm::vec3 light1Ambient(0.4f, 0.4f, 0.4f);
    glm::vec3 light1Diffuse(0.9f, 0.1f, 0.1f);
    glm::vec3 light1Specular(0.9f, 0.1f, 0.1f);
    Lightning* light1Src = new Lightning(light1Position, light1Ambient, light1Diffuse, light1Specular);

    glm::vec3 light2Position(10, -10, -10);
    glm::vec3 light2Ambient(0.1f, 0.9f, 0.1f);
    glm::vec3 light2Diffuse(0.1f, 0.9f, 0.1f);
    glm::vec3 light2Specular(0.1f, 0.9f, 0.1f);
    Lightning* light2Src = new Lightning(light2Position, light2Ambient, light2Diffuse, light2Specular);

    glm::vec3 lightsPositions[2] = { light1Position,light2Position };
    glm::mat3 lightsIntMats[2] = { light1Src->getIntensitiesMatrix(), light2Src->getIntensitiesMatrix()};
    //object creation
    Object* obj = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\donny.fbx","C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\donny.png");
    //Object* obj = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\dragon.obj", "");
    //obj->print();

    //Object* obj2 = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\boombox_4k.fbx\\boombox_4k.fbx", "C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\boombox_4k.fbx\\textures\\boombox_diff_4k.jpg");

    unsigned int VAOid = obj->getMesh(0).createBuffer(); //VAO already knows about its VBO
    obj->generateTexture();
    
    /*unsigned int VAOid2 = obj2->getMesh(0).createBuffer();
    obj2->generateTexture();*/


    Shader* vertexShader = new VertexShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\vertexShader1.vert");
    vertexShader->createShader();
    vertexShader->compileShader();

    Shader* fragmentShader = new FragmentShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\fragmentShader1.frag");
    fragmentShader->createShader();
    fragmentShader->compileShader();


    ShaderProgramme* shaderProgramme = new ShaderProgramme(vertexShader, fragmentShader);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model2 = glm::translate(model, glm::vec3(5.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(8.0f));
    shaderProgramme->setUniformsVertexShader(model, cam->getLookAtMatrix(), cam->getProjectionMatrix());
    glm::vec3 material(0.8f, 0.8f, 0.8f);
    shaderProgramme->setUniformsFragmentShader(2, lightsPositions, lightsIntMats, material, cam->getCameraPosition(),true);
    shaderProgramme->checkLinkingSuccess();

    //render loop with double buffering
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, cam);

        //changing uniforms if needed
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(8.0f));

        shaderProgramme->setUniformsVertexShader(model, cam->getLookAtMatrix(), cam->getProjectionMatrix());
        shaderProgramme->setUniformsFragmentShader(2, lightsPositions, lightsIntMats, material, cam->getCameraPosition(),true);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, obj->getMesh(0).getNumOfVertices());
        obj->drawObject(shaderProgramme,model,cam->getLookAtMatrix(),cam->getProjectionMatrix());
        //obj2->drawObject(shaderProgramme, model2, cam->getLookAtMatrix(), cam->getProjectionMatrix());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete obj;
    delete cam;
    delete light1Src;
    delete light2Src;
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
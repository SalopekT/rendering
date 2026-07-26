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

const int MAX_LIGHTS = 16;

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
    glm::vec3 camPosition(-10, -10, 5);
    glm::vec3 camCenter(0, 0, 0);
    glm::vec3 camViewUp(0, 0, 1);
    Camera* cam = new PerspectiveCamera(camPosition, camCenter, camViewUp, 45.0f, width, height, 0.01f, 100.0f);

    //light settings, multiple lights
    glm::vec3 light1Position(10, 10, 10);
    glm::vec3 light1Ambient(0.2f, 0.2f, 0.2f);  //ambient component of the first light source is main ambient component
    glm::vec3 light1Diffuse(0.1f, 0.1f, 0.1f);
    glm::vec3 light1Specular(0.1f, 0.1f, 0.1f);
    Lightning* light1Src = new Lightning(1,light1Position, light1Ambient, light1Diffuse, light1Specular);

    glm::vec3 light2Position(5, -5, 10);
    glm::vec3 light2Ambient(0.5f, 0.5f, 0.5f);
    glm::vec3 light2Diffuse(0.9f, 0.9f, 0.9f);
    glm::vec3 light2Specular(0.9f, 0.9f, 0.9f);
    Lightning* light2Src = new Lightning(2,light2Position, light2Ambient, light2Diffuse, light2Specular);

    glm::vec3 lightsPositions[MAX_LIGHTS] = {}; 
    glm::mat3 lightsIntMats[MAX_LIGHTS] = {};
    glm::vec3 lightsDirections[MAX_LIGHTS] = {};
    float lightsCutoffAngles[MAX_LIGHTS] = {}; //this is the inner cone, outer cone will be this+alpha lets say
    int lightTypes[16] = {};


    lightsPositions[0] = light1Position;
    lightsPositions[1] = light2Position;

    lightsIntMats[0] = light1Src->getIntensitiesMatrix();
    lightsIntMats[1] = light2Src->getIntensitiesMatrix();

    lightTypes[0] = 1;
    lightTypes[1] = 2;

    lightsDirections[1] = glm::vec3(-5.0, 5.0, -10.0);
    lightsCutoffAngles[1] = 0.9597;

    int numLights = 2;

    //creating a floor object so i can show shadow mapping
    double size = 100.0;

    std::vector<std::shared_ptr<Vertex3d>> vertices = {
        std::make_shared<Vertex3d>(-size, -size, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0), 
        std::make_shared<Vertex3d>(size, -size, 0.0,   0.0, 0.0, 1.0,   1.0, 0.0), 
        std::make_shared<Vertex3d>(size,  size, 0.0,   0.0, 0.0, 1.0,   1.0, 1.0), 
        std::make_shared<Vertex3d>(-size,  size, 0.0,   0.0, 0.0, 1.0,   0.0, 1.0)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,  
        0, 2, 3 
    };

    std::vector<Face3d> faces = {
        Face3d(vertices.at(0), vertices.at(1), vertices.at(2)),
        Face3d(vertices.at(0), vertices.at(2), vertices.at(3))
    };

    Mesh floor(vertices, faces, indices);
    Object* floorObj = new Object(std::vector<Mesh>{floor});
    unsigned int VAOidFloor = floorObj->getMesh(0).createBuffer(); //VAO already knows about its VBO
    floorObj->generateTexture();


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
    /*glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 0.0, 5.0));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(8.0f));*/
    shaderProgramme->setUniformsVertexShader(model, cam->getLookAtMatrix(), cam->getProjectionMatrix());
    glm::vec3 material(0.8f, 0.8f, 0.8f);
    shaderProgramme->setUniformsFragmentShader(2, lightTypes,lightsPositions, lightsIntMats,
        lightsDirections, lightsCutoffAngles,material, cam->getCameraPosition(),false);
    shaderProgramme->checkLinkingSuccess();


    //framebuffer for shadow mapping
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    //glDeleteFramebuffers(1, &fbo);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 1000, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);




    //render loop with double buffering
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, cam);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glDrawArrays(GL_TRIANGLES, 0, obj->getMesh(0).getNumOfVertices());
        //changing uniforms if needed
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(0.0, 0.0, 2.0));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1, 0, 0));
        //model = glm::scale(model, glm::vec3(2.0f));
        shaderProgramme->setUniformsVertexShader(model, cam->getLookAtMatrix(), cam->getProjectionMatrix());
        shaderProgramme->setUniformsFragmentShader(numLights, lightTypes,lightsPositions, lightsIntMats,
                                            lightsDirections, lightsCutoffAngles,material, cam->getCameraPosition(), true);
        obj->drawObject(*shaderProgramme,cam->getLookAtMatrix(),cam->getProjectionMatrix());


        //changing uniforms if needed
        /*model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));*/
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
        //model = glm::scale(model, glm::vec3(8.0f));
        shaderProgramme->setUniformsVertexShader(model, cam->getLookAtMatrix(), cam->getProjectionMatrix());
        shaderProgramme->setUniformsFragmentShader(numLights, lightTypes,lightsPositions, lightsIntMats,
                                            lightsDirections, lightsCutoffAngles,material, cam->getCameraPosition(), false);
        floorObj->drawObject(*shaderProgramme, cam->getLookAtMatrix(), cam->getProjectionMatrix());
        //obj2->drawObject(shaderProgramme, model2, cam->getLookAtMatrix(), cam->getProjectionMatrix());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete obj;
    delete floorObj;
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
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam->moveCamera(5);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam->moveCamera(6);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam->moveCamera(7);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam->moveCamera(8);
    }

}
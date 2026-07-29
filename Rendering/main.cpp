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
#include "Scene.hpp"
#include "Shader.hpp"
#include "Lightning.hpp"
#include "SpotLightning.hpp"
#include "Renderer.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, std::shared_ptr<Camera> cam);

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
    std::shared_ptr<Camera> cam = std::make_shared<PerspectiveCamera>(camPosition, camCenter, camViewUp, 45.0f, width, height, 0.01f, 100.0f);
    //light settings, multiple lights
    glm::vec3 light1Position(10, 10, 10);
    glm::vec3 light1Ambient(0.2f, 0.2f, 0.2f);  //ambient component of the first light source is main ambient component
    glm::vec3 light1Diffuse(0.1f, 0.1f, 0.1f);
    glm::vec3 light1Specular(0.1f, 0.1f, 0.1f);
    std::shared_ptr<Lightning> light1src = std::make_shared<Lightning>(1, light1Position, light1Ambient, light1Diffuse, light1Specular);

    glm::vec3 light2Position(5, -5, 10);
    glm::vec3 light2Ambient(0.5f, 0.5f, 0.5f);
    glm::vec3 light2Diffuse(0.9f, 0.9f, 0.9f);
    glm::vec3 light2Specular(0.9f, 0.9f, 0.9f);
    //Lightning* light2Src = new Lightning(2,light2Position, light2Ambient, light2Diffuse, light2Specular);
    std::shared_ptr<Lightning> light2src = std::make_shared<SpotLightning>(light2Position, light2Ambient, light2Diffuse, light2Specular,
                                                                            glm::vec3(-5.0, 5.0, -10.0), 0.9597);


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
    floor.createBuffer();
    std::shared_ptr<Object> floorObj = std::make_shared<Object>(std::vector<Mesh>{floor});
    floorObj->setMaterialCoeffs(glm::vec3{ 0.8f, 0.8f, 0.8f });
    floorObj->setTextureFlag(false);


    //object creation
    //Object* obj = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\donny.fbx","C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\donny.png");
    std::shared_ptr<Object> obj = std::make_shared<Object>("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\donny.fbx", "C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\donny.png");
    obj->setMaterialCoeffs(glm::vec3{ 0.8f, 0.8f, 0.8f });
    obj->setTextureFlag(true);
    obj->getTransform().setScale(glm::vec3{ 3.0f, 3.0f, 3.0f });
    obj->getTransform().setPosition(glm::vec3{ 0.0f, 0.0f, 3.0f });
    //Object* obj = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\dragon.obj", "");
    //obj->print();

    //Object* obj2 = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\boombox_4k.fbx\\boombox_4k.fbx", "C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\boombox_4k.fbx\\textures\\boombox_diff_4k.jpg");

    //unsigned int VAOid = obj->getMesh(0).createBuffer(); //VAO already knows about its VBO
    obj->generateTexture();
    
    /*unsigned int VAOid2 = obj2->getMesh(0).createBuffer();
    obj2->generateTexture();*/

    //scene creation
    std::shared_ptr<Scene> scene= std::make_shared<Scene>();
    scene->addObject(obj);
    scene->addObject(floorObj);
    scene->addLight(light1src);
    scene->addLight(light2src);
    scene->setCamera(cam);

    Shader* vertexShader = new VertexShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\vertexShader1.vert");
    vertexShader->createShader();
    vertexShader->compileShader();
    Shader* fragmentShader = new FragmentShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\fragmentShader1.frag");
    fragmentShader->createShader();
    fragmentShader->compileShader();
    std::shared_ptr<ShaderProgramme> shaderProgramme = std::make_shared<ShaderProgramme>(vertexShader, fragmentShader);
   
    shaderProgramme->checkLinkingSuccess();


    Renderer* renderer = new Renderer(scene, shaderProgramme);

    //framebuffer for shadow mapping
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //render loop with double buffering
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, cam);
        
        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderer->renderScene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete renderer;
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, std::shared_ptr<Camera> cam)
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
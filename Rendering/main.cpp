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
#include <glm/gtc/type_ptr.hpp>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
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

    glViewport(0, 0, 800, 600);

    //camera settings
    glm::vec3 camPosition(10, 0, 0);
    glm::vec3 camCenter(0, 0, 0);
    glm::vec3 camViewUp(0, 1, 0);
    Camera* cam = new PerspectiveCamera(camPosition, camCenter, camViewUp, 45.0f, 800, 600, 0.01f, 100.0f);

    //object creation
    Object* obj = new Object("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Objects\\teapot.obj");
    //obj->print();

    unsigned int VAOid = obj->getMesh(0).createBuffer(); //VAO already knows about its VBO


    Shader* vertexShader = new VertexShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\vertexShader1.vert");
    vertexShader->createShader();
    vertexShader->compileShader();

    Shader* fragmentShader = new FragmentShader("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\fragmentShader1.frag");
    fragmentShader->createShader();
    fragmentShader->compileShader();


    ShaderProgramme* shaderProgramme = new ShaderProgramme(vertexShader, fragmentShader);
    shaderProgramme->setUniforms(glm::mat4(1.0f), cam->getLookAtMatrix(), cam->getProjectionMatrix());
    shaderProgramme->checkLinkingSuccess();

    //render loop with double buffering
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, obj->getMesh(0).getNumOfVertices());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete obj;
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

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
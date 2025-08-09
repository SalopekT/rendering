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

    //generating and binding vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //generating vertex buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->getMesh(0).getNumOfVertices()*3*sizeof(float), obj->getMesh(0).getVerticesArray(), GL_STATIC_DRAW);

    //linking vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //creating a vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //reading shader source code
    std::ifstream file("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\vertexShader1.vert");
    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string shaderSrcCode = buffer.str();
    const char* shaderSrcCodePtr = shaderSrcCode.c_str();

    //compiling a shader
    glShaderSource(vertexShader, 1, &shaderSrcCodePtr, NULL);
    glCompileShader(vertexShader);

    //checking if compiling succeded
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //creating a fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //reading shader from source code
    std::ifstream file2("C:\\Users\\tinsa\\Projects\\Graphics\\Rendering\\Rendering\\Rendering\\fragmentShader1.frag");
    std::ostringstream buffer2;
    buffer2 << file2.rdbuf();
    std::string shaderSrcCode2 = buffer2.str();
    const char* shaderSrcCodePtr2 = shaderSrcCode2.c_str();

    //compiling a shader
    glShaderSource(fragmentShader, 1, &shaderSrcCodePtr2, NULL);
    glCompileShader(fragmentShader);

    //checking if compiling succeded
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //creating a shader programme
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //linking shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //setting uniforms
    int viewMat = glGetUniformLocation(shaderProgram, "view");
    int projectionMat = glGetUniformLocation(shaderProgram, "projection");
    int modelMat = glGetUniformLocation(shaderProgram, "model");

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(cam->getLookAtMatrix()));
    glUniformMatrix4fv(projectionMat, 1, GL_FALSE, glm::value_ptr(cam->getProjectionMatrix()));
    glUniformMatrix4fv(modelMat, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING FAILED\n" << infoLog << std::endl;
    }

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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
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
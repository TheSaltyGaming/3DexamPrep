#include "Shader.h"
#include "ShaderFileLoader.h"
#include <iostream>
#include <map>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Line.h"
#include "Math.h"
#include "Mesh.h"
#include "glm/mat4x3.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void CameraView(std::vector<unsigned> shaderPrograms, glm::mat4 trans, glm::mat4 projection);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void DrawObjects(unsigned VAO, Shader ShaderProgram);

Math math;

// settings

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

struct colorStruct
{
    glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 green = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 white = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 black = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
    glm::vec3 cyan = glm::vec3(0.0f, 1.0f, 1.0f);
    glm::vec3 magenta = glm::vec3(1.0f, 0.0f, 1.0f);
    glm::vec3 orange = glm::vec3(1.0f, 0.5f, 0.0f);
    glm::vec3 purple = glm::vec3(0.5f, 0.0f, 0.5f);
    glm::vec3 grey = glm::vec3(0.5f, 0.5f, 0.5f);
};

colorStruct colors;
///Delta time variables
///--------------------
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

///Mouse Input Variables
///---------------------
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

std::string vfs = ShaderLoader::LoadShaderFromFile("Triangle.vert");
std::string fs = ShaderLoader::LoadShaderFromFile("Triangle.frag");

/// THINGS TO DO
/// TODO: Refactor code to seperate functions
/// TODO: TERRAIN
/// TODO: PLANE
/// TODO: COLLISION. Just give it all AABB for now.
/// TODO: MOVEMENT?
/// TODO: USE SHADERFILELOADER



Camera MainCamera;

Mesh testingBox;
Mesh testingTriangle;
Mesh testingSquare;
Mesh testingPyramid;
Mesh testingSphere;
Mesh testingPlayerCollision;


std::vector<unsigned> shaderPrograms;

void DrawObjects(unsigned VAO, Shader ShaderProgram)
{
    ShaderProgram.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    testingBox.Draw(ShaderProgram.ID);
    testingTriangle.Draw(ShaderProgram.ID);
    testingSquare.Draw(ShaderProgram.ID);
    testingPyramid.Draw(ShaderProgram.ID);
    testingSphere.Draw(ShaderProgram.ID);
    

    

    
}



void render(GLFWwindow* window, Shader ourShader, unsigned VAO)
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
    
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        glLineWidth(12);
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


        CameraView(shaderPrograms, model, projection);

        //slowly move testing box to the right
        testingBox.globalPosition.x += deltaTime * 1.1f;
        //slowly rotate testing box
        testingBox.globalRotation.y += deltaTime * 100.1f;

        testingTriangle.globalPosition.x -= deltaTime * 2.1f;

        testingSquare.globalPosition.y += deltaTime * 1.1f;

        testingSphere.globalPosition.z += deltaTime * 1.1f;
        testingSphere.globalRotation.y += deltaTime * 1.1f;

        //testingPyramid.globalPosition.x -= deltaTime * 1.1f;

        testingPlayerCollision.globalPosition = MainCamera.cameraPos;

        testingPlayerCollision.CheckCollision(&testingPyramid);

        float speed = 1.0f * deltaTime; // Adjust the speed as necessary

        //math.moveObject(&testingTriangle, &testingBox, speed);
        
        
        
        
        // input
        // -----
        processInput(window);
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // render the triangle
        DrawObjects(VAO, ourShader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void SetupMeshes()
{
    testingBox = Mesh(Cube, 1.0f, colors.red );
    testingTriangle = Mesh(Triangle, 1.0f, colors.green);
    testingSquare = Mesh(Square, 1.0f, colors.blue);
    testingPyramid = Mesh(Pyramid, 1.0f, colors.magenta);
    testingSphere = Mesh(Sphere, 1.0f, 10, colors.orange);

    testingPlayerCollision = Mesh(Cube, 1.0f, colors.yellow);
}

int main()
{
    //Print out shader
    // std::cout << "Vfs " << vfs.c_str() << std::endl;
    // std::cout << "fs " << fs.c_str() << std::endl;'
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test Win", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback); // Set the mouse callback function

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------

    //Shader ourShader("Triangle.vert", "Triangle.frag"); // you can name your shader files however you like
    Shader ourShader("VertShaderOld.vert", "FragShaderOld.frag"); // you can name your shader files however you like

    shaderPrograms.push_back(ourShader.ID);
    
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  0.5f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    /// SETUP MESHES HER
    SetupMeshes();
    
    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
    

    
    //RENDER FUNCTION HERE!!!!!!!
    render(window, ourShader, VAO);

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    glm::vec3 cameraFrontXZ = glm::normalize(glm::vec3(MainCamera.cameraFront.x, 0.0f, MainCamera.cameraFront.z));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        MainCamera.cameraPos += cameraSpeed * cameraFrontXZ;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        MainCamera.cameraPos -= cameraSpeed * cameraFrontXZ;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        MainCamera.cameraPos -= glm::normalize(glm::cross(MainCamera.cameraFront, MainCamera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        MainCamera.cameraPos += glm::normalize(glm::cross(MainCamera.cameraFront, MainCamera.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        MainCamera.cameraPos += cameraSpeed * MainCamera.cameraUp; // Move camera up
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        MainCamera.cameraPos -= cameraSpeed * MainCamera.cameraUp; // Move camera down
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        ;
        MainCamera.cameraPos.y += 0.01;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Prevents sudden jump in camera movement
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;
 
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
 
    MainCamera.yaw   += xoffset;
    MainCamera.pitch += yoffset;
 
    if(MainCamera.pitch > 89.0f)
        MainCamera.pitch = 89.0f;
    if(MainCamera.pitch < -89.0f)
        MainCamera.pitch = -89.0f;
 
    glm::vec3 direction;
    direction.x = cos(glm::radians(MainCamera.yaw)) * cos(glm::radians(MainCamera.pitch));
    direction.y = sin(glm::radians(MainCamera.pitch));
    direction.z = sin(glm::radians(MainCamera.yaw)) * cos(glm::radians(MainCamera.pitch));
    MainCamera.cameraFront = glm::normalize(direction);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/// \brief Handles Camera view functions
/// \param shaderPrograms vector of all shaders
/// \param trans transformation matrix
/// \param projection projection matrix
void CameraView(std::vector<unsigned> shaderPrograms, glm::mat4 trans, glm::mat4 projection)
{
    for (unsigned shaderProgram : shaderPrograms)
    {
        glUseProgram(shaderProgram);

        int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        MainCamera.tick();

        glm::mat4 view;
        view = glm::lookAt(MainCamera.cameraPos, MainCamera.cameraPos + MainCamera.cameraFront, MainCamera.cameraUp);

        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        // Pass the transformation matrix to the vertex shader
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    }
}
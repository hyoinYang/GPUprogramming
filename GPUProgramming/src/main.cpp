#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>

#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    // build and compile shaders
   // -------------------------
   //Shader shader("shader/wall.vs", "shader/wall.fs", "shader/wall.gs");
    Shader shader("shader/wall.vs", "shader/wall.fs");
    //Shader shaderSingleColor("shader/3.1.blending.vs", "shader/2.stencil_single_color.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions          // texture Coords 
         5.0f, -1.0f,  5.0f,  20.0f, 0.0f,
        -5.0f, -1.0f,  5.0f,  0.0f, 0.0f,
        -5.0f, -1.0f, -5.0f,  0.0f, 20.0,

         5.0f, -1.0f,  5.0f,  20.0, 0.0f,
        -5.0f, -1.0f, -5.0f,  0.0f, 20.0,
         5.0f, -1.0f, -5.0f,  20.0, 20.0
    };

    // Front wall vertices
    float wallFrontVertices[] = {
        // positions          // texture Coords 
        // Front face
        -5.0f, -1.0f,  5.0f,  0.0f, 0.0f,
         5.0f, -1.0f,  5.0f,  10.0f, 0.0f,
         5.0f,  1.0f,  5.0f,  10.0f, 10.0f,
        -5.0f, -1.0f,  5.0f,  0.0f, 0.0f,
         5.0f,  1.0f,  5.0f,  10.0f, 10.0f,
        -5.0f,  1.0f,  5.0f,  0.0f, 10.0f
    };

    // Right wall vertices
    float wallRightVertices[] = {
        // positions          // texture Coords 
        // Right face
         5.0f, -1.0f,  5.0f,  0.0f, 0.0f,
         5.0f, -1.0f, -5.0f,  10.0f, 0.0f,
         5.0f,  1.0f, -5.0f,  10.0f, 10.0f,
         5.0f, -1.0f,  5.0f,  0.0f, 0.0f,
         5.0f,  1.0f, -5.0f,  10.0f, 10.0f,
         5.0f,  1.0f,  5.0f,  0.0f, 10.0f
    };

    // Left wall vertices
    float wallLeftVertices[] = {
        // positions          // texture Coords 
        // Left face
        -5.0f, -1.0f, -5.0f,  0.0f, 0.0f,
        -5.0f, -1.0f,  5.0f,  10.0f, 0.0f,
        -5.0f,  1.0f,  5.0f,  10.0f, 10.0f,
        -5.0f, -1.0f, -5.0f,  0.0f, 0.0f,
        -5.0f,  1.0f,  5.0f,  10.0f, 10.0f,
        -5.0f,  1.0f, -5.0f,  0.0f, 10.0f
    };

    // Back wall vertices
    float wallBackVertices[] = {
        // positions          // texture Coords 
        // Back face
        -5.0f, -1.0f, -5.0f,  0.0f, 0.0f,
         5.0f, -1.0f, -5.0f,  10.0f, 0.0f,
         5.0f,  1.0f, -5.0f,  10.0f, 10.0f,
        -5.0f, -1.0f, -5.0f,  0.0f, 0.0f,
         5.0f,  1.0f, -5.0f,  10.0f, 10.0f,
        -5.0f,  1.0f, -5.0f,  0.0f, 10.0f
    };

    // Ceiling vertices
    float ceilingVertices[] = {
        // positions          // texture Coords 
         5.0f,  1.0f,  5.0f,  5.0f, 5.0f,
        -5.0f,  1.0f,  5.0f,  0.0f, 5.0f,
        -5.0f,  1.0f, -5.0f,  0.0f, 0.0f,

         5.0f,  1.0f,  5.0f,  5.0f, 5.0f,
        -5.0f,  1.0f, -5.0f,  0.0f, 0.0f,
         5.0f,  1.0f, -5.0f,  5.0f, 0.0f
    };



    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // wallFront VAO
    unsigned int wallVAO, wallVBO;
    glGenVertexArrays(1, &wallVAO);
    glGenBuffers(1, &wallVBO);
    glBindVertexArray(wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallFrontVertices), wallFrontVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Right wall VAO
    unsigned int wallRightVAO, wallRightVBO;
    glGenVertexArrays(1, &wallRightVAO);
    glGenBuffers(1, &wallRightVBO);
    glBindVertexArray(wallRightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallRightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallRightVertices), wallRightVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Left wall VAO
    unsigned int wallLeftVAO, wallLeftVBO;
    glGenVertexArrays(1, &wallLeftVAO);
    glGenBuffers(1, &wallLeftVBO);
    glBindVertexArray(wallLeftVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallLeftVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallLeftVertices), wallLeftVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Back wall VAO
    unsigned int wallBackVAO, wallBackVBO;
    glGenVertexArrays(1, &wallBackVAO);
    glGenBuffers(1, &wallBackVBO);
    glBindVertexArray(wallBackVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallBackVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallBackVertices), wallBackVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Ceiling VAO
    unsigned int ceilingVAO, ceilingVBO;
    glGenVertexArrays(1, &ceilingVAO);
    glGenBuffers(1, &ceilingVBO);
    glBindVertexArray(ceilingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ceilingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ceilingVertices), ceilingVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));















    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float classplaneVertices[] = {
        // positions          // texture Coords 
        -5.0f, -1.0f, 4.0f,  20.0f, 0.0f,
        -9.0f, -1.0f, 4.0f,  0.0f, 0.0f,
        -9.0f, -1.0f, 0.0f,  0.0f, 20.0,
        -5.0f, -1.0f, 4.0f,  20.0, 0.0f,
        -9.0f, -1.0f, 0.0f,  0.0f, 20.0,
        -5.0f, -1.0f, 0.0f,  20.0, 20.0
    };

    // Front wall vertices
    float classwallFrontVertices[] = {
        // positions          // texture Coords 
        // Front face
        -9.0f, -1.0f, 4.0f,  0.0f, 0.0f,
        -5.0f, -1.0f, 4.0f,  10.0f, 0.0f,
        -5.0f,  1.0f, 4.0f,  10.0f, 10.0f,
        -9.0f, -1.0f, 4.0f,  0.0f, 0.0f,
        -5.0f,  1.0f, 4.0f,  10.0f, 10.0f,
        -9.0f,  1.0f, 4.0f,  0.0f, 10.0f
    };

    // Right wall vertices
    float classwallRightVertices[] = {
        // positions          // texture Coords 
        // Right face
        -5.0f, -1.0f, 4.0f,  0.0f, 0.0f,
        -5.0f, -1.0f, 0.0f,  10.0f, 0.0f,
        -5.0f,  1.0f, 0.0f,  10.0f, 10.0f,
        -5.0f, -1.0f, 4.0f,  0.0f, 0.0f,
        -5.0f,  1.0f, 0.0f,  10.0f, 10.0f,
        -5.0f,  1.0f, 4.0f,  0.0f, 10.0f
    };

    // Left wall vertices
    float classwallLeftVertices[] = {
        // positions          // texture Coords 
        // Left face
        -9.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -9.0f, -1.0f, 4.0f,  10.0f, 0.0f,
        -9.0f,  1.0f, 4.0f,  10.0f, 10.0f,
        -9.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -9.0f,  1.0f, 4.0f,  10.0f, 10.0f,
        -9.0f,  1.0f, 0.0f,  0.0f, 10.0f
    };

    // Back wall vertices
    float classwallBackVertices[] = {
        // positions          // texture Coords 
        // Back face
        -9.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -5.0f, -1.0f, 0.0f,  10.0f, 0.0f,
        -5.0f,  1.0f, 0.0f,  10.0f, 10.0f,
        -9.0f, -1.0f, 0.0f,  0.0f, 0.0f,
        -5.0f,  1.0f, 0.0f,  10.0f, 10.0f,
        -9.0f,  1.0f, 0.0f,  0.0f, 10.0f
    };

    // Ceiling vertices
    float classceilingVertices[] = {
        // positions          // texture Coords 
        -5.0f,  1.0f, 4.0f,  5.0f, 5.0f,
        -9.0f,  1.0f, 4.0f,  0.0f, 5.0f,
        -9.0f,  1.0f, 0.0f,  0.0f, 0.0f,
        -5.0f,  1.0f, 4.0f,  5.0f, 5.0f,
        -9.0f,  1.0f, 0.0f,  0.0f, 0.0f,
        -5.0f,  1.0f, 0.0f,  5.0f, 0.0f
    };



    // plane VAO
    unsigned int classplaneVAO, classplaneVBO;
    glGenVertexArrays(1, &classplaneVAO);
    glGenBuffers(1, &classplaneVBO);
    glBindVertexArray(classplaneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, classplaneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(classplaneVertices), &classplaneVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // wallFront VAO
    unsigned int classwallVAO, classwallVBO;
    glGenVertexArrays(1, &classwallVAO);
    glGenBuffers(1, &classwallVBO);
    glBindVertexArray(classwallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, classwallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(classwallFrontVertices), classwallFrontVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Right wall VAO
    unsigned int classwallRightVAO, classwallRightVBO;
    glGenVertexArrays(1, &classwallRightVAO);
    glGenBuffers(1, &classwallRightVBO);
    glBindVertexArray(classwallRightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, classwallRightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(classwallRightVertices), classwallRightVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Left wall VAO
    unsigned int classwallLeftVAO, classwallLeftVBO;
    glGenVertexArrays(1, &classwallLeftVAO);
    glGenBuffers(1, &classwallLeftVBO);
    glBindVertexArray(classwallLeftVAO);
    glBindBuffer(GL_ARRAY_BUFFER, classwallLeftVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(classwallLeftVertices), classwallLeftVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Back wall VAO
    unsigned int classwallBackVAO, classwallBackVBO;
    glGenVertexArrays(1, &classwallBackVAO);
    glGenBuffers(1, &classwallBackVBO);
    glBindVertexArray(classwallBackVAO);
    glBindBuffer(GL_ARRAY_BUFFER, classwallBackVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(classwallBackVertices), classwallBackVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // Ceiling VAO
    unsigned int classceilingVAO, classceilingVBO;
    glGenVertexArrays(1, &classceilingVAO);
    glGenBuffers(1, &classceilingVBO);
    glBindVertexArray(classceilingVAO);
    glBindBuffer(GL_ARRAY_BUFFER, classceilingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(classceilingVertices), classceilingVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));




    // door vertices
    float doorVertices[] = {
        // positions          // texture Coords 
        // Back face
        -5.0f, -1.0f, -5.0f,  0.0f, 0.0f,
         5.0f, -1.0f, -5.0f,  10.0f, 0.0f,
         5.0f,  1.0f, -5.0f,  10.0f, 10.0f,
        -5.0f, -1.0f, -5.0f,  0.0f, 0.0f,
         5.0f,  1.0f, -5.0f,  10.0f, 10.0f,
        -5.0f,  1.0f, -5.0f,  0.0f, 10.0f
    };
    
    // door VAO
    unsigned int doorVAO, doorVBO;
    glGenVertexArrays(1, &doorVAO);
    glGenBuffers(1, &doorVBO);
    glBindVertexArray(doorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(doorVertices), doorVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    








    // load textures
    // -------------
    unsigned int floorTexture = loadTexture("resources/textures/floor.png");
    unsigned int wallTexture = loadTexture("resources/textures/wall.png");
    unsigned int ceilingTexture = loadTexture("resources/textures/ceiling.png");

    // transparent object locations
    // --------------------------------
    vector<glm::vec3> trasparentObject
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // don't forget to clear the stencil buffer!

        // set uniforms
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers. We set its mask to 0x00 to not write to the stencil buffer.
        glStencilMask(0x00);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // wall
        glBindVertexArray(wallVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(wallRightVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(wallLeftVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(wallBackVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // ceiling
        glBindVertexArray(ceilingVAO);
        glBindTexture(GL_TEXTURE_2D, ceilingTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);


        // floor
        glBindVertexArray(classplaneVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // wall
        glBindVertexArray(classwallVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(classwallRightVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(classwallLeftVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glBindVertexArray(classwallBackVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // ceiling
        glBindVertexArray(classceilingVAO);
        glBindTexture(GL_TEXTURE_2D, ceilingTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);













        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &planeVAO);

    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        /*
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

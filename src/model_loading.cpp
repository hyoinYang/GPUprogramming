#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define USE_FLASH_SHADER

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
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
    Shader shader("shader/wall.vs", "shader/wall.fs");

    // 복도 좌표 설정

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions          // texture Coords 
         7.5f, -1.0f,  7.5f,  20.0f, 0.0f,
        -7.5f, -1.0f,  7.5f,  0.0f, 0.0f,
        -7.5f, -1.0f, -7.5f,  0.0f, 20.0,

         7.5f, -1.0f,  7.5f,  20.0, 0.0f,
        -7.5f, -1.0f, -7.5f,  0.0f, 20.0,
         7.5f, -1.0f, -7.5f,  20.0, 20.0
    };

    // Front wall vertices
    float wallFrontVertices[] = {
        // positions          // texture Coords 
        // Front face
        -7.5f, -1.0f,  7.5f,  0.0f, 0.0f,
         7.5f, -1.0f,  7.5f,  10.0f, 0.0f,
         7.5f,  2.0f,  7.5f,  10.0f, 10.0f,
        -7.5f, -1.0f,  7.5f,  0.0f, 0.0f,
         7.5f,  2.0f,  7.5f,  10.0f, 10.0f,
        -7.5f,  2.0f,  7.5f,  0.0f, 10.0f
    };

    // Right wall vertices
    float wallRightVertices[] = {
        // positions          // texture Coords 
        // Right face
         7.5f, -1.0f,  7.5f,  0.0f, 0.0f,
         7.5f, -1.0f, -7.5f,  10.0f, 0.0f,
         7.5f,  2.0f, -7.5f,  10.0f, 10.0f,
         7.5f, -1.0f,  7.5f,  0.0f, 0.0f,
         7.5f,  2.0f, -7.5f,  10.0f, 10.0f,
         7.5f,  2.0f,  7.5f,  0.0f, 10.0f
    };

    // Left wall vertices
    float wallLeftVertices[] = {
        // positions          // texture Coords 
        // Left face
        -7.5f, -1.0f, -7.5f,  0.0f, 0.0f,
        -7.5f, -1.0f,  7.5f,  10.0f, 0.0f,
        -7.5f,  2.0f,  7.5f,  10.0f, 10.0f,
        -7.5f, -1.0f, -7.5f,  0.0f, 0.0f,
        -7.5f,  2.0f,  7.5f,  10.0f, 10.0f,
        -7.5f,  2.0f, -7.5f,  0.0f, 10.0f
    };

    // Back wall vertices
    float wallBackVertices[] = {
        // positions          // texture Coords 
        // Back face
        -7.5f, -1.0f, -7.5f,  0.0f, 0.0f,
         7.5f, -1.0f, -7.5f,  10.0f, 0.0f,
         7.5f,  2.0f, -7.5f,  10.0f, 10.0f,
        -7.5f, -1.0f, -7.5f,  0.0f, 0.0f,
         7.5f,  2.0f, -7.5f,  10.0f, 10.0f,
        -7.5f,  2.0f, -7.5f,  0.0f, 10.0f
    };

    // Ceiling vertices
    float ceilingVertices[] = {
        // positions          // texture Coords 
         7.5f,  2.0f,  7.5f,  5.0f, 5.0f,
        -7.5f,  2.0f,  7.5f,  0.0f, 5.0f,
        -7.5f,  2.0f, -7.5f,  0.0f, 0.0f,

         7.5f,  2.0f,  7.5f,  5.0f, 5.0f,
        -7.5f,  2.0f, -7.5f,  0.0f, 0.0f,
         7.5f,  2.0f, -7.5f,  5.0f, 0.0f
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

    // 교실 좌표 설정

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float classplaneVertices[] = {
        // positions          // texture Coords 
        -7.5f, -1.0f,  6.0f,  10.0f, 10.0f,
        -13.5f, -1.0f,  6.0f,  0.0f, 10.0f,
        -13.5f, -1.0f,  0.0f,  0.0f,  0.0f,
        -7.5f, -1.0f,  6.0f,  10.0f, 10.0f,
        -13.5f, -1.0f,  0.0f,  0.0f,  0.0f,
        -7.5f, -1.0f,  0.0f,  10.0f,  0.0f
    };

    // Front wall vertices
    float classwallFrontVertices[] = {
        // positions          // texture Coords 
        // Front face
        -13.5f, -1.0f, 6.0f,  0.0f, 0.0f,
        -7.5f, -1.0f, 6.0f,  10.0f, 0.0f,
        -7.5f,  2.0f, 6.0f,  10.0f, 10.0f,
        -13.5f, -1.0f, 6.0f,  0.0f, 0.0f,
        -7.5f,  2.0f, 6.0f,  10.0f, 10.0f,
        -13.5f,  2.0f, 6.0f,  0.0f, 10.0f
    };

    // Right wall vertices
    float classwallRightVertices[] = {
        // positions          // texture Coords 
        // Right face
        -7.5f, -1.0f, 6.0f,  0.0f, 0.0f,
        -7.5f, -1.0f, 0.0f,  10.0f, 0.0f,
        -7.5f,  2.0f, 0.0f,  10.0f, 10.0f,
        -7.5f, -1.0f, 6.0f,  0.0f, 0.0f,
        -7.5f,  2.0f, 0.0f,  10.0f, 10.0f,
        -7.5f,  2.0f, 6.0f,  0.0f, 10.0f
    };

    // Left wall vertices
    float classwallLeftVertices[] = {
        // positions          // texture Coords 
        // Left face
        -13.5f, -1.0f, 0.0f,  0.0f, 0.0f,
        -13.5f, -1.0f, 6.0f,  10.0f, 0.0f,
        -13.5f,  2.0f, 6.0f,  10.0f, 10.0f,
        -13.5f, -1.0f, 0.0f,  0.0f, 0.0f,
        -13.5f,  2.0f, 6.0f,  10.0f, 10.0f,
        -13.5f,  2.0f, 0.0f,  0.0f, 10.0f
    };

    // Back wall vertices
    float classwallBackVertices[] = {
        // positions          // texture Coords 
        // Back face
        -13.5f, -1.0f, 0.0f,  0.0f, 0.0f,
        -7.5f, -1.0f, 0.0f,  10.0f, 0.0f,
        -7.5f,  2.0f, 0.0f,  10.0f, 10.0f,
        -13.5f, -1.0f, 0.0f,  0.0f, 0.0f,
        -7.5f,  2.0f, 0.0f,  10.0f, 10.0f,
        -13.5f,  2.0f, 0.0f,  0.0f, 10.0f
    };

    // Ceiling vertices
    float classceilingVertices[] = {
        // positions          // texture Coords 
        -7.5f,  2.0f, 6.0f,  5.0f, 5.0f,
        -13.5f,  2.0f, 6.0f,  0.0f, 5.0f,
        -13.5f,  2.0f, 0.0f,  0.0f, 0.0f,
        -7.5f,  2.0f, 6.0f,  5.0f, 5.0f,
        -13.5f,  2.0f, 0.0f,  0.0f, 0.0f,
        -7.5f,  2.0f, 0.0f,  5.0f, 0.0f
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

    // load textures
    // -------------
    unsigned int floorTexture = loadTexture("resources/textures/floor.png");
    unsigned int wallTexture = loadTexture("resources/textures/wall.png");
    unsigned int ceilingTexture = loadTexture("resources/textures/ceiling.png");

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    // 모델 설정

    Shader ourShader("shader/1.model_loading.vs", "shader/1.model_loading.fs");
    Model computer("resources/computer/scene.gltf");
    Model door("resources/door/scene.gltf");
    Model whiteboard("resources/whiteboard/scene.gltf");
    Model clock("resources/clock/scene.gltf");
    Model table("resources/table/scene.gltf");
    Model chair("resources/chair/scene.gltf");
    Model blind("resources/blind/scene.gltf");
    Model windows("resources/window/scene.gltf");
    Model screen("resources/screen/scene.gltf");
    Model air_conditioner("resources/air_conditioner/scene.gltf");

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

        // 모델 그리기

        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        /* 
        //skeleton
        glm::mat4 model_ = glm::mat4(1.0f);
        model_ = glm::translate(model_, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model_ = glm::scale(model_, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        model_ = glm::rotate(model_, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        ourShader.setMat4("model", model_);
        door.Draw(ourShader);
        */
     
        // render the loaded model

        // 교실

        glm::mat4 model_front_door = glm::mat4(1.0f);
        model_front_door = glm::translate(model_front_door, glm::vec3(-7.45f, 0.0f, 1.0f)); // translate it down so it's at the center of the scene
        model_front_door = glm::scale(model_front_door, glm::vec3(0.04f, 0.03f, 0.025f));	// it's a bit too big for our scene, so scale it down
        model_front_door = glm::rotate(model_front_door, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
        model_front_door = glm::rotate(model_front_door, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
        ourShader.setMat4("model", model_front_door);
        door.Draw(ourShader);

        glm::mat4 model_back_door = glm::mat4(1.0f);
        model_back_door = glm::translate(model_back_door, glm::vec3(-7.55f, 0.0f, 5.0f)); // translate it down so it's at the center of the scene
        model_back_door = glm::scale(model_back_door, glm::vec3(0.04f, 0.03f, 0.025f));	// it's a bit too big for our scene, so scale it down
        model_back_door = glm::rotate(model_back_door, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        model_back_door = glm::rotate(model_back_door, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
        ourShader.setMat4("model", model_back_door);
        door.Draw(ourShader);

        for (int i = 0; i < 2; i++) {
            glm::mat4 model_window1 = glm::mat4(1.0f);
            model_window1 = glm::translate(model_window1, glm::vec3(-7.53f, 0.85f, 2.2f + i * 1.6)); // translate it down so it's at the center of the scene
            model_window1 = glm::scale(model_window1, glm::vec3(0.5f, 0.3f, 0.7f));	// it's a bit too big for our scene, so scale it down
            model_window1 = glm::rotate(model_window1, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
            ourShader.setMat4("model", model_window1);
            windows.Draw(ourShader);
        }

        glm::mat4 model_table = glm::mat4(1.0f);
        model_table = glm::translate(model_table, glm::vec3(-10.5f, -1.0f, 1.5f));  // translate it down so it's at the center of the scene
        model_table = glm::scale(model_table, glm::vec3(1.1f, 0.8f, 0.8f));	 // it's a bit too big for our scene, so scale it down
        model_table = glm::rotate(model_table, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
        model_table = glm::rotate(model_table, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
        ourShader.setMat4("model", model_table);
        table.Draw(ourShader);

        glm::mat4 model_computer = glm::mat4(1.0f);
        model_computer = glm::translate(model_computer, glm::vec3(-10.5f, -0.45f, 1.52f)); // translate it down so it's at the center of the scene
        model_computer = glm::scale(model_computer, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        model_computer = glm::rotate(model_computer, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
        ourShader.setMat4("model", model_computer);
        computer.Draw(ourShader);

        glm::mat4 model_chair = glm::mat4(1.0f);
        model_chair = glm::translate(model_chair, glm::vec3(-10.5f, -0.7f, 1.25f)); // translate it down so it's at the center of the scene
        model_chair = glm::scale(model_chair, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
        model_chair = glm::rotate(model_chair, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        model_chair = glm::rotate(model_chair, glm::radians(90.0f), glm::vec3(0.0, -1.0, 0.0));
        ourShader.setMat4("model", model_chair);
        chair.Draw(ourShader);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 4; j++) {
                glm::mat4 model_table = glm::mat4(1.0f);
                model_table = glm::translate(model_table, glm::vec3(-12.5f + i * 2, -1.0f, 2.0f + j * 1));  // translate it down so it's at the center of the scene
                model_table = glm::scale(model_table, glm::vec3(1.1f, 0.8f, 0.8f));	 // it's a bit too big for our scene, so scale it down
                model_table = glm::rotate(model_table, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
                model_table = glm::rotate(model_table, glm::radians(90.0f), glm::vec3(0.0, 0.0, -1.0));
                ourShader.setMat4("model", model_table);
                table.Draw(ourShader);

                glm::mat4 model_left_computer = glm::mat4(1.0f);
                model_left_computer = glm::translate(model_left_computer, glm::vec3(-12.8f + i * 2, -0.45f, 1.97f + j * 1)); // translate it down so it's at the center of the scene
                model_left_computer = glm::scale(model_left_computer, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
                ourShader.setMat4("model", model_left_computer);
                computer.Draw(ourShader);

                glm::mat4 model_right_computer = glm::mat4(1.0f);
                model_right_computer = glm::translate(model_right_computer, glm::vec3(-12.2f + i * 2, -0.45f, 1.97f + j * 1)); // translate it down so it's at the center of the scene
                model_right_computer = glm::scale(model_right_computer, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
                ourShader.setMat4("model", model_right_computer);
                computer.Draw(ourShader);

                glm::mat4 model_left_chair = glm::mat4(1.0f);
                model_left_chair = glm::translate(model_left_chair, glm::vec3(-12.8f + i * 2, -0.7f, 2.2f + j * 1)); // translate it down so it's at the center of the scene
                model_left_chair = glm::scale(model_left_chair, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
                model_left_chair = glm::rotate(model_left_chair, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
                ourShader.setMat4("model", model_left_chair);
                chair.Draw(ourShader);

                glm::mat4 model_right_chair = glm::mat4(1.0f);
                model_right_chair = glm::translate(model_right_chair, glm::vec3(-12.2f + i * 2, -0.7f, 2.2f + j * 1)); // translate it down so it's at the center of the scene
                model_right_chair = glm::scale(model_right_chair, glm::vec3(0.3f, 0.3f, 0.3f));	// it's a bit too big for our scene, so scale it down
                model_right_chair = glm::rotate(model_right_chair, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
                ourShader.setMat4("model", model_right_chair);
                chair.Draw(ourShader);
            }
        }

        glm::mat4 model_whiteboard = glm::mat4(1.0f);
        model_whiteboard = glm::translate(model_whiteboard, glm::vec3(-10.5f, -0.6f, 0.1f)); // translate it down so it's at the center of the scene
        model_whiteboard = glm::scale(model_whiteboard, glm::vec3(0.02f, 0.011f, 0.01f));	// it's a bit too big for our scene, so scale it down
        //model_whiteboard = glm::rotate(model_whiteboard, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
        ourShader.setMat4("model", model_whiteboard);
        whiteboard.Draw(ourShader);

        glm::mat4 model_clock = glm::mat4(1.0f);
        model_clock = glm::translate(model_clock, glm::vec3(-8.5f, 1.2f, 0.1f)); // translate it down so it's at the center of the scene
        model_clock = glm::scale(model_clock, glm::vec3(0.7f, 0.7f, 0.7f));	// it's a bit too big for our scene, so scale it down
        model_clock = glm::rotate(model_clock, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
        ourShader.setMat4("model", model_clock);
        clock.Draw(ourShader);

        glm::mat4 model_screen = glm::mat4(1.0f);
        model_screen = glm::translate(model_screen, glm::vec3(-12.3f, 1.1f, 0.6f)); // translate it down so it's at the center of the scene
        model_screen = glm::scale(model_screen, glm::vec3(2.1f, 0.8f, 1.5f));	// it's a bit too big for our scene, so scale it down
        model_screen = glm::rotate(model_screen, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
        ourShader.setMat4("model", model_screen);
        screen.Draw(ourShader);

        for (int i = 0; i < 5; i++) {
            glm::mat4 model_window2 = glm::mat4(1.0f);
            model_window2 = glm::translate(model_window2, glm::vec3(-13.47f, 0.3f, 0.9f + i * 1.1)); // translate it down so it's at the center of the scene
            model_window2 = glm::scale(model_window2, glm::vec3(0.5f, 0.3f, 0.7f));	// it's a bit too big for our scene, so scale it down
            model_window2 = glm::rotate(model_window2, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
            ourShader.setMat4("model", model_window2);
            windows.Draw(ourShader);

            glm::mat4 model_blind = glm::mat4(1.0f);
            model_blind = glm::translate(model_blind, glm::vec3(-13.4f, 1.2f, 0.9f + i * 1.1)); // translate it down so it's at the center of the scene
            model_blind = glm::scale(model_blind, glm::vec3(1.1f, 0.7f, 1.0f));	// it's a bit too big for our scene, so scale it down
            model_blind = glm::rotate(model_blind, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
            ourShader.setMat4("model", model_blind);
            blind.Draw(ourShader);
        }

        for (int i = 0; i < 2; i++) {
            glm::mat4 model_air_conditioner = glm::mat4(1.0f);
            model_air_conditioner = glm::translate(model_air_conditioner, glm::vec3(-10.5f, 1.95f, 1.5f + i * 3)); // translate it down so it's at the center of the scene
            model_air_conditioner = glm::scale(model_air_conditioner, glm::vec3(0.4f, 0.4f, 0.4f));	// it's a bit too big for our scene, so scale it down
            ourShader.setMat4("model", model_air_conditioner);
            air_conditioner.Draw(ourShader);
        }

        // 복도 그리기

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
        /* 
        glBindVertexArray(wallVAO);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        */

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

        /*
        // ceiling
        glBindVertexArray(ceilingVAO);
        glBindTexture(GL_TEXTURE_2D, ceilingTexture);
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        */ 

        // 교실 그리기

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

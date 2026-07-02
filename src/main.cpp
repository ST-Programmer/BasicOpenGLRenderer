#include "camera.hpp"
#include "mesh.hpp"
#include "objs/Vertex.hpp"
#include "renderer.hpp"
#include "shaderprogram.hpp"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <vector>
#include "model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    GL_CALL(glViewport(0, 0, width, height));
}

void HandleInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(void)
{
    if (!glfwInit())
    {
        ERR("Failed To Initialize GLFW");
        return -1;
    }

    int windowWidth = 1280;
    int windowHeight = 720;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Window", NULL, NULL);

    if (!window)
    {
        ERR("Failed To Create GLFW Window");
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        ERR("Failed To Load OpenGL Function Pointers (GLAD)");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    io.IniFilename = "assets/imgui.ini";

    // Position (x, y, z), UV (u, v)
    // Order:F, B, L, R, U, D

    std::vector<Vertex> vertices = {
        // +Z face (front)
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 1.0f}},

        // -Z face (back)
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 1.0f}},

        // -X face (left)
        {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 1.0f}},

        // +X face (right)
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 1.0f}},

        // +Y face (top)
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 1.0f}},

        // -Y face (bottom)
        {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.5f, 0.31f}, {0.0f, 1.0f}},
    };

    std::vector<unsigned int> indices = {// Front
                                         0, 1, 2, 2, 3, 0,

                                         // Back
                                         4, 5, 6, 6, 7, 4,

                                         // Left
                                         8, 9, 10, 10, 11, 8,

                                         // Right
                                         12, 13, 14, 14, 15, 12,

                                         // Top
                                         16, 17, 18, 18, 19, 16,

                                         // Bottom
                                         20, 21, 22, 22, 23, 20};

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Renderer renderer;
    Camera camera{window, 1280, 720, glm::vec3(0.0f, 0.0f, 3.0f)};

    glm::vec3 lightColor(NORMALIZERGB(255, 255, 255));

    ShaderProgram shaderProgram("./src/shaders/vertex.glsl", "./src/shaders/fragment.glsl");
    ShaderProgram lightingProgram("./src/shaders/vertexlight.glsl",
                                  "./src/shaders/fragmentlight.glsl");

    std::vector<std::shared_ptr<Texture>> cubeTextures;
    cubeTextures.emplace_back(std::make_shared<Texture>(Texture("assets/planks.png", "diffuse")));
    cubeTextures.emplace_back(
        std::make_shared<Texture>(Texture("assets/planksSpec.png", "specular")));

    std::vector<std::shared_ptr<Texture>> lightTextures;
    lightTextures.emplace_back(std::make_shared<Texture>(Texture("assets/planks.png", "diffuse")));
    lightTextures.emplace_back(
        std::make_shared<Texture>(Texture("assets/planksSpec.png", "specular")));

    Mesh lightCube{vertices, indices, std::move(lightTextures)};

    Model backpack("assets/backpack/backpack.obj");

    float lastTime = glfwGetTime();
    float currentTime = 0;
    float deltaTime = 0;

    glm::vec3 lightPos(1.0f, 2.0f, -2.0f);
    glm::vec3 cubePos(0.0f, 0.0f, 0.0f);
    float lightScale = 0.25f;

    int fps = 0;
    int frameCount = 0;
    float fpsTimer = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("ImGui Window");

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        frameCount++;
        fpsTimer += deltaTime;

        if (fpsTimer >= 1.0f)
        {
            fps = frameCount;
            frameCount = 0;
            fpsTimer -= 1.0f;
        }

        ImGui::Text("FPS: %d", fps);

        renderer.Clear(10, 41, 71, 255, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        HandleInput(window);

        camera.Movement(window, deltaTime);
        camera.CreateMatrix(45.0f, 0.1f, 100.0f);

        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(lightScale, lightScale, lightScale));

        glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePos);
        glm::mat3 normalMatrix(glm::transpose(glm::inverse(model)));

        ImGui::SliderFloat3("Cube Position", glm::value_ptr(cubePos), -10.0f, 10.0f);
        ImGui::SliderFloat3("Light Position", glm::value_ptr(lightPos), -10.0f, 10.0f);
        ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));

        lightingProgram.SetMat4("u_Model", lightModel);
        lightingProgram.SetVec3("u_LightSourceColor", lightColor);

        shaderProgram.SetMat4("u_Model", model);
        shaderProgram.SetVec3("u_LightSourceColor", lightColor);
        shaderProgram.SetVec3("u_LightPos", lightPos);
        shaderProgram.SetMat3("u_NormalMatrix", normalMatrix);
        shaderProgram.SetVec3("u_CameraPos", camera.getPos());

        backpack.Draw(renderer, shaderProgram, camera);
        renderer.DrawMesh(lightCube, lightingProgram, camera);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

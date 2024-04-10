#include "Renderer.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void LightmapBaker::Renderer::Renderer::Initialize()
{
    GLFWInitialize();
    ImGuiInitialize();
}

void LightmapBaker::Renderer::Renderer::Update()
{
    GLFWUpdate();
    ImGuiUpdate();
}

void LightmapBaker::Renderer::Renderer::Render()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    GLFWRender();
    ImGuiRender();

    glfwSwapBuffers(glfwWindow);
}

bool LightmapBaker::Renderer::Renderer::WindowShouldClose()
{
    return glfwWindowShouldClose(glfwWindow);
}

void LightmapBaker::Renderer::Renderer::Exit()
{
    GLFWExit();
    ImGuiExit();
}

void LightmapBaker::Renderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void LightmapBaker::Renderer::Renderer::GLFWInitialize()
{
    glfwInit();
    glfwWindow = glfwCreateWindow(SCW, SCH, "Lightmap Baker", 0, 0);

    quadricObj = gluNewQuadric();

    glfwMakeContextCurrent(glfwWindow);

    glfwSetKeyCallback(glfwWindow, KeyCallBack);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

void LightmapBaker::Renderer::Renderer::GLFWUpdate()
{
    glfwPollEvents();
}

void LightmapBaker::Renderer::Renderer::GLFWRender()
{
    // Perspective Camera Setting
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect_ratio = ((float)SCW) / SCH;
    gluPerspective(80, (1.f / aspect_ratio), 0.0f, 200.0f); // FOV, ratio, zNear, zFar

    // Model Setting
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    static float angle = 0.0f;
    angle += 0.5f;
    camAngleX += 0.5f;

    //glRotatef(angle, 0, 0, 1);

    float camX = distance * -sinf(camAngleX * (M_PI / 180)) * cosf((camAngleY) * (M_PI / 180));
    float camY = distance * -sinf((camAngleY) * (M_PI / 180));
    float camZ = -distance * cosf((camAngleX) * (M_PI / 180)) * cosf((camAngleY) * (M_PI / 180));
    gluLookAt(camX, camY, camZ,
        0.0, 0.0, 0.0,    // Look at point
        0.0, 1.0, 0.0);   // Up vector

    glColor3f(1, 1, 0);
    gluQuadricDrawStyle(quadricObj, GLU_LINE);
    gluCylinder(quadricObj, 10, 10, 30, 10, 10);
}

void LightmapBaker::Renderer::Renderer::GLFWExit()
{
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void LightmapBaker::Renderer::Renderer::ImGuiInitialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

void LightmapBaker::Renderer::Renderer::ImGuiUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Controller");
    if (ImGui::Button("Calculate Radiosity"))
    {
    }
    ImGui::End();
}

void LightmapBaker::Renderer::Renderer::ImGuiRender()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LightmapBaker::Renderer::Renderer::ImGuiExit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

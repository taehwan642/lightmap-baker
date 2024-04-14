#include "Renderer.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

float LightmapBaker::Renderer::Renderer::cameraDistance = 500.0f;

void LightmapBaker::Renderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void LightmapBaker::Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void LightmapBaker::Renderer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    LightmapBaker::Renderer::Renderer::cameraDistance += yoffset;
}

void LightmapBaker::Renderer::Renderer::Initialize()
{
    GLFWInitialize();
    ImGuiInitialize();
}

void LightmapBaker::Renderer::Renderer::Update()
{
    toolState.Update();
    GLFWUpdate();
    ImGuiUpdate();
}

void LightmapBaker::Renderer::Renderer::Render()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void LightmapBaker::Renderer::Renderer::AddRenderMesh(const std::shared_ptr<Mesh>& mesh)
{
    renderMeshList.push_back(mesh);
}

void LightmapBaker::Renderer::Renderer::GLFWInitialize()
{
    glfwInit();
    glfwWindow = glfwCreateWindow(screenWidth, screenHeight, "Lightmap Baker", 0, 0);

    glfwGetCursorPos(glfwWindow, &mousePositionX, &mousePositionY);

    glfwMakeContextCurrent(glfwWindow);

    glfwSetKeyCallback(glfwWindow, KeyCallBack);

    glfwGetFramebufferSize(glfwWindow, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);

    glfwSetScrollCallback(glfwWindow, ScrollCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    __int64 periodFrequency;
    QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrequency);
    QueryPerformanceCounter((LARGE_INTEGER*)&lastDeltaQuery);
    queryScale = 1.0 / (double)periodFrequency;
}

void LightmapBaker::Renderer::Renderer::GLFWUpdate()
{
    glfwPollEvents();

    QueryPerformanceCounter((LARGE_INTEGER*)&currentDeltaQuery);
    deltaTime = (double)(currentDeltaQuery - lastDeltaQuery) * queryScale;
    lastDeltaQuery = currentDeltaQuery;

    static double xPos, yPos;
    glfwGetCursorPos(glfwWindow, &xPos, &yPos);
    if (glfwGetMouseButton(glfwWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        cameraAngleX += mousePositionX - xPos;
        cameraAngleY += mousePositionY - yPos;

        const double polarCap = (M_PI / 2.0f - 0.00001f) * (180 / M_PI);
        if (cameraAngleY > polarCap) cameraAngleY = polarCap;
        if (cameraAngleY < -polarCap) cameraAngleY = -polarCap;
    }
    mousePositionX = xPos;
    mousePositionY = yPos;
}

void LightmapBaker::Renderer::Renderer::GLFWRender()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = screenHeight / (float)screenWidth;
    gluPerspective(80, aspectRatio, 0.01f, cameraDistance * 2);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = cameraDistance * -sinf(cameraAngleX * (M_PI / 180)) * cosf((cameraAngleY) * (M_PI / 180));
    float camY = cameraDistance * -sinf((cameraAngleY) * (M_PI / 180));
    float camZ = -cameraDistance * cosf((cameraAngleX) * (M_PI / 180)) * cosf((cameraAngleY) * (M_PI / 180));
    gluLookAt(camX, camY, camZ,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    for (int i = 0; i < renderMeshList.size(); ++i)
    {
        renderMeshList[i]->Render();
    }

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void LightmapBaker::Renderer::Renderer::GLFWExit()
{
    for (int i = 0; i < renderMeshList.size(); ++i)
    {
        renderMeshList[i]->Destroy();
    }

    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void LightmapBaker::Renderer::Renderer::ImGuiInitialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    toolState.UpdateCurrentState(ToolStateEnum::BEFORE_RADIOSITY_CALCULATION);
}

void LightmapBaker::Renderer::Renderer::ImGuiUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    toolState.RenderCurrentUI();
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

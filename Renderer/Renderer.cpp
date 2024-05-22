#include "Renderer.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "UI.hpp"
#include "Camera.hpp"
#include "ToolState.hpp"

void LightmapBaker::Renderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    using namespace LightmapBaker::Renderer;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        Renderer& renderer = Renderer::GetInstance();
        MainCamera& camera = MainCamera::GetInstance();
        switch (key)
        {
        case GLFW_KEY_W:
            camera.position.z += 100.0f * renderer.deltaTime * camera.speed;
            break;
        case GLFW_KEY_A:
            camera.position.x += 100.0f * renderer.deltaTime * camera.speed;
            break;
        case GLFW_KEY_S:
            camera.position.z -= 100.0f * renderer.deltaTime * camera.speed;
            break;
        case GLFW_KEY_D:
            camera.position.x -= 100.0f * renderer.deltaTime * camera.speed;
            break;
        case GLFW_KEY_Q:
            camera.position.y += 100.0f * renderer.deltaTime * camera.speed;
            break;
        case GLFW_KEY_E:
            camera.position.y -= 100.0f * renderer.deltaTime * camera.speed;
            break;
        case GLFW_KEY_Z:
            camera.speed += 50.0f * renderer.deltaTime;
            break;
        case GLFW_KEY_X:
            camera.speed -= 50.0f * renderer.deltaTime;
            break;
        default:
            break;
        }
    }
}

void LightmapBaker::Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Renderer& renderer = Renderer::Renderer::GetInstance();
    renderer.SetFramebufferSize(width, height);
}

void LightmapBaker::Renderer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    MainCamera& camera = MainCamera::GetInstance();
    camera.distance += yoffset * camera.speed;
}

void LightmapBaker::Renderer::Renderer::Update()
{
    ToolState::GetInstance().Update();
    GLFWUpdate();
    ImGuiUpdate();
}

void LightmapBaker::Renderer::Renderer::BeforeRender()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LightmapBaker::Renderer::Renderer::Render()
{
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

void LightmapBaker::Renderer::Renderer::AddRenderMesh(SplitterType type, const std::shared_ptr<Mesh>& mesh)
{
    std::shared_ptr<Splitter>& splitter = type == SplitterType::LEFT ? leftSplitter : rightSplitter;
    splitter->AddRenderMesh(mesh);
}

void LightmapBaker::Renderer::Renderer::RemoveRenderMesh(const std::shared_ptr<Mesh>& mesh)
{
    for (std::vector<std::shared_ptr<Mesh>>::iterator iter = renderMeshList.begin(); iter != renderMeshList.end();)
    {
        if (iter->get() == mesh.get())
        {
            iter = renderMeshList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

LightmapBaker::Renderer::Renderer::Renderer()
{
    GLFWInitialize();
    ImGuiInitialize();

    leftSplitter = std::make_shared<Splitter>(SplitterType::LEFT);
    rightSplitter = std::make_shared<Splitter>(SplitterType::RIGHT);

    leftSplitter->SetFrameBufferWidthHeight(framebufferWidth, framebufferHeight);
    rightSplitter->SetFrameBufferWidthHeight(framebufferWidth, framebufferHeight);
}

void LightmapBaker::Renderer::Renderer::RemoveRenderMesh(SplitterType type, const std::shared_ptr<Mesh>& mesh)
{
    std::shared_ptr<Splitter>& splitter = type == SplitterType::LEFT ? leftSplitter : rightSplitter;
    splitter->RemoveRenderMesh(mesh);
}

void LightmapBaker::Renderer::Renderer::ClearRenderMesh(SplitterType type)
{
    std::shared_ptr<Splitter>& splitter = type == SplitterType::LEFT ? leftSplitter : rightSplitter;
    splitter->ClearRenderMesh();
}

void LightmapBaker::Renderer::Renderer::SetFramebufferSize(int width, int height)
{
    framebufferWidth = width;
    framebufferHeight = height;
    leftSplitter->SetFrameBufferWidthHeight(width, height);
    rightSplitter->SetFrameBufferWidthHeight(width, height);
}

void LightmapBaker::Renderer::Renderer::SetSplitterRenderIndex(SplitterType type, int index)
{
    std::shared_ptr<Splitter>& splitter = type == SplitterType::LEFT ? leftSplitter : rightSplitter;
    splitter->SetRenderIndex(index);
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
        MainCamera& camera = MainCamera::GetInstance();
        camera.angle.x += mousePositionX - xPos;
        camera.angle.y += mousePositionY - yPos;

        const double polarCap = (M_PI / 2.0f - 0.00001f) * (180 / M_PI);
        if (camera.angle.y > polarCap) camera.angle.y = polarCap;
        if (camera.angle.y < -polarCap) camera.angle.y = -polarCap;
    }
    mousePositionX = xPos;
    mousePositionY = yPos;

    glfwGetWindowSize(glfwWindow, &UI::screenWidth, &UI::screenHeight);
}

void LightmapBaker::Renderer::Renderer::GLFWRender()
{
    leftSplitter->Render();
    rightSplitter->Render();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    MainCamera& camera = MainCamera::GetInstance();
    float aspectRatio = screenHeight / (float)screenWidth;
    gluPerspective(80, aspectRatio, 0.01f, camera.distance * 2 + 100);

    camera.Render();

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    for (int i = 0; i < renderMeshList.size(); ++i)
    {
        renderMeshList[i]->Render({ ToolState::GetInstance().GetMeshDrawMode() });
    }

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    ToolState& toolState = ToolState::GetInstance();
    toolState.UpdateCurrentState(ToolStateEnum::BEFORE_RADIOSITY_CALCULATION);
}

void LightmapBaker::Renderer::Renderer::ImGuiUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void LightmapBaker::Renderer::Renderer::ImGuiRender()
{
    ToolState::GetInstance().RenderCurrentUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LightmapBaker::Renderer::Renderer::ImGuiExit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

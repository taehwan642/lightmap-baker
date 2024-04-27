#include "Renderer.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

LightmapBaker::Renderer::Camera LightmapBaker::Renderer::Renderer::camera = LightmapBaker::Renderer::Camera();
double LightmapBaker::Renderer::Renderer::deltaTime = 0.0;

void LightmapBaker::Renderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    using namespace LightmapBaker::Renderer;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            Renderer::camera.position.z += 100.0f * Renderer::deltaTime * Renderer::camera.speed;
            break;
        case GLFW_KEY_A:
            Renderer::camera.position.x += 100.0f * Renderer::deltaTime * Renderer::camera.speed;
            break;
        case GLFW_KEY_S:
            Renderer::camera.position.z -= 100.0f * Renderer::deltaTime * Renderer::camera.speed;
            break;
        case GLFW_KEY_D:
            Renderer::camera.position.x -= 100.0f * Renderer::deltaTime * Renderer::camera.speed;
            break;
        case GLFW_KEY_Q:
            Renderer::camera.position.y += 100.0f * Renderer::deltaTime * Renderer::camera.speed;
            break;
        case GLFW_KEY_E:
            Renderer::camera.position.y -= 100.0f * Renderer::deltaTime * Renderer::camera.speed;
            break;
        case GLFW_KEY_Z:
            Renderer::camera.speed += 50.0f * Renderer::deltaTime;
            break;
        case GLFW_KEY_X:
            Renderer::camera.speed -= 50.0f * Renderer::deltaTime;
            break;
        default:
            break;
        }
    }
}

void LightmapBaker::Renderer::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void LightmapBaker::Renderer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    LightmapBaker::Renderer::Renderer::camera.distance += yoffset * Renderer::camera.speed;
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

void LightmapBaker::Renderer::Renderer::BeforeRender()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void LightmapBaker::Renderer::Renderer::Render()
{
    GLFWRender();
    GLFWRenderBeforeBaked();
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
        camera.angle.x += mousePositionX - xPos;
        camera.angle.y += mousePositionY - yPos;

        const double polarCap = (M_PI / 2.0f - 0.00001f) * (180 / M_PI);
        if (camera.angle.y > polarCap) camera.angle.y = polarCap;
        if (camera.angle.y < -polarCap) camera.angle.y = -polarCap;
    }
    mousePositionX = xPos;
    mousePositionY = yPos;

    glfwGetWindowSize(glfwWindow, &toolState.curScreenWidth, &toolState.curScreenHeight);
}

void LightmapBaker::Renderer::Renderer::GLFWRender()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, framebufferWidth, framebufferHeight, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);

    glColor3f(1, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(toolState.GetCompareXPosition(), 0);
    glVertex2f(toolState.GetCompareXPosition(), framebufferHeight);
    glVertex2f(0, framebufferHeight);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = screenHeight / (float)screenWidth;
    gluPerspective(80, aspectRatio, 0.01f, camera.distance * 2 + 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = (camera.distance * -sinf(camera.angle.x * (M_PI / 180)) * cosf((camera.angle.y) * (M_PI / 180))) + camera.position.x;
    float camY = (camera.distance * -sinf((camera.angle.y) * (M_PI / 180))) + camera.position.y;
    float camZ = (-camera.distance * cosf((camera.angle.x) * (M_PI / 180)) * cosf((camera.angle.y) * (M_PI / 180))) + camera.position.z;
    gluLookAt(camX, camY, camZ,
        camera.position.x, camera.position.y, camera.position.z,
        0.0, 1.0, 0.0);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    for (int i = 0; i < renderMeshList.size(); ++i)
    {
        renderMeshList[i]->Render();
    }

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void LightmapBaker::Renderer::Renderer::GLFWRenderBeforeBaked()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, framebufferWidth, framebufferHeight, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);

    glColor3f(0, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(toolState.GetCompareXPosition(), 0);
    glVertex2f(framebufferWidth, 0);
    glVertex2f(framebufferWidth, framebufferHeight);
    glVertex2f(toolState.GetCompareXPosition(), framebufferHeight);
    glEnd();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspectRatio = screenHeight / (float)screenWidth;
    gluPerspective(80, aspectRatio, 0.01f, camera.distance * 2 + 100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = (camera.distance * -sinf(camera.angle.x * (M_PI / 180)) * cosf((camera.angle.y) * (M_PI / 180))) + camera.position.x;
    float camY = (camera.distance * -sinf((camera.angle.y) * (M_PI / 180))) + camera.position.y;
    float camZ = (-camera.distance * cosf((camera.angle.x) * (M_PI / 180)) * cosf((camera.angle.y) * (M_PI / 180))) + camera.position.z;
    gluLookAt(camX, camY, camZ,
        camera.position.x, camera.position.y, camera.position.z,
        0.0, 1.0, 0.0);

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    // Put here radiocity-before-calculated model.
    glTranslatef(0, 100, 0);
    for (int i = 0; i < renderMeshList.size(); ++i)
    {
        renderMeshList[i]->Render();
    }

    glDisable(GL_STENCIL_TEST);
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

    toolState.SetFrame(1.0 / deltaTime);
}

void LightmapBaker::Renderer::Renderer::ImGuiRender()
{
    toolState.RenderCurrentUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LightmapBaker::Renderer::Renderer::ImGuiExit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

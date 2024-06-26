#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <vector>

#include "GLFW/glfw3.h"
#include "GLUT/glut.h"

#include "ToolState.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

#include "../Utility/Singleton.hpp"

namespace LightmapBaker
{
namespace Renderer
{
    class Renderer : public Utility::Singleton<Renderer>
    {
    private:
        Renderer() = default;
        ~Renderer() = default;
        friend class Utility::Singleton<Renderer>;

    private:
        GLFWwindow* glfwWindow = nullptr;

        const char* glslVersion = "#version 130";

        __int64 currentDeltaQuery;
        __int64 lastDeltaQuery;
        double queryScale = 0;

        double mousePositionX = 0;
        double mousePositionY = 0;

        void GLFWInitialize();
        void GLFWUpdate();
        void GLFWRender();
        void GLFWExit();

    public:
        std::vector<std::shared_ptr<Mesh>> renderMeshList;

    private:
        void ImGuiInitialize();
        void ImGuiUpdate();
        void ImGuiRender();
        void ImGuiExit();

    public:
        void Initialize();
        void Update();
        void BeforeRender();
        void Render();
        bool WindowShouldClose();
        void Exit();
        void AddRenderMesh(const std::shared_ptr<Mesh>& mesh);
        void RemoveRenderMesh(const std::shared_ptr<Mesh>& mesh);

    public:
        const int screenWidth = 640;
        const int screenHeight = 480;
        int framebufferWidth;
        int framebufferHeight;
        ToolState toolState;

    public:
        double deltaTime;
        Camera camera;
    };
    void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}
}

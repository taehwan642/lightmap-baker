#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include "GLFW/glfw3.h"
#include "GLUT/glut.h"

#include "Mesh.hpp"
#include "Splitter.hpp"

#include "../Utility/Singleton.hpp"

namespace LightmapBaker
{
namespace Renderer
{
    class Renderer : public Utility::Singleton<Renderer>
    {
    private:
        friend class Utility::Singleton<Renderer>;

    private:
        GLFWwindow* glfwWindow = nullptr;

        const char* glslVersion = "#version 130";

        __int64 currentDeltaQuery;
        __int64 lastDeltaQuery;
        double queryScale = 0;

        double mousePositionX = 0;
        double mousePositionY = 0;

    private:
        std::vector<std::shared_ptr<Mesh>> renderMeshList;
        std::shared_ptr<Splitter> leftSplitter;
        std::shared_ptr<Splitter> rightSplitter;

    private:
        void GLFWInitialize();
        void GLFWUpdate();
        void GLFWRender();
        void GLFWExit();

    private:
        void ImGuiInitialize();
        void ImGuiUpdate();
        void ImGuiRender();
        void ImGuiExit();
        void AddRenderMesh(const std::shared_ptr<Mesh>& mesh);
        void RemoveRenderMesh(const std::shared_ptr<Mesh>& mesh);

    public:
        Renderer();

        void Update();
        void BeforeRender();
        void Render();
        bool WindowShouldClose();
        void Exit();
        void AddRenderMesh(SplitterType type, const std::shared_ptr<Mesh>& mesh);
        void RemoveRenderMesh(SplitterType type, const std::shared_ptr<Mesh>& mesh);
        void ClearRenderMesh(SplitterType type);
        void SetFramebufferSize(int width, int height);
        void SetSplitterRenderIndex(SplitterType type, int index);

    public:
        const int screenWidth = 640;
        const int screenHeight = 480;
        int framebufferWidth;
        int framebufferHeight;

    public:
        double deltaTime;
    };
    void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
}
}

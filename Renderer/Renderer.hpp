#pragma once
#include <GLFW/glfw3.h>

namespace LightmapBaker
{
namespace Renderer
{
    class Renderer
    {
    private:
        GLFWwindow* glfwWindow;
        const char* glslVersion = "#version 130";

        void GLFWInitialize();
        void GLFWUpdate();
        void GLFWRender();
        void GLFWExit();

    private:
        void ImGuiInitialize();
        void ImGuiUpdate();
        void ImGuiRender();
        void ImGuiExit();

    public:
        void Initialize();
        void Update();
        void Render();
        bool WindowShouldClose();
        void Exit();
    };
    void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
}
}
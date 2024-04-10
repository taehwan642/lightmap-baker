#pragma once
#include <memory>
#include <GLFW/glfw3.h>

namespace LightmapBaker
{
namespace Renderer
{
    class Renderer
    {
    private:
        GLFWwindow* glfwWindow;
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
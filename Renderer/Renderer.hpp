#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include "GLFW/glfw3.h"
#include "GLUT/glut.h"


namespace LightmapBaker
{
namespace Renderer
{
    class Renderer
    {
    private:
        GLFWwindow* glfwWindow;
        const char* glslVersion = "#version 130";

        // ~~ Camera Settings ~~ //
        GLUquadric* quadricObj = nullptr;
        float camAngleX = 0;
        float camAngleY = 0;
        float distance = 100;

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
        const int SCW = 640;
        const int SCH = 480;

        void Initialize();
        void Update();
        void Render();
        bool WindowShouldClose();
        void Exit();
    };
    void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
}
}
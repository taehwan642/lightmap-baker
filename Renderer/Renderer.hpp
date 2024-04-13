#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#include "GLFW/glfw3.h"
#include "GLUT/glut.h"

#include "ToolState.hpp"

namespace LightmapBaker
{
namespace Renderer
{
    class Renderer
    {
    private:
        const int screenWidth = 640;
        const int screenHeight = 480;
        GLFWwindow* glfwWindow = nullptr;

        const char* glslVersion = "#version 130";

        GLUquadric* quadricObj = nullptr;

        float cameraAngleX = 0;
        float cameraAngleY = 0;
        float cameraDistance = 100;

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

    public:
        ToolState toolState;
    };
    void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
}
}
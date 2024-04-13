#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

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
        int framebufferWidth;
        int framebufferHeight;
        GLFWwindow* glfwWindow = nullptr;

        const char* glslVersion = "#version 130";

        GLUquadric* quadricObj = nullptr;

        float cameraAngleX = 0;
        float cameraAngleY = 0;
        float cameraDistance = 100;

        __int64 currentDeltaQuery;
        __int64 lastDeltaQuery;
        double deltaTime = 0;
        double queryScale = 0;

        double mousePositionX = 0;
        double mousePositionY = 0;

        void GLFWInitialize();
        void GLFWUpdate();
        void GLFWRender();
        void GLFWExit();

    private:
        void ImGuiInitialize();
        void ImGuiUpdate();
        void ImGuiRender();
        void ImGuiExit();

    private:
        ToolState toolState;

    public:
        void Initialize();
        void Update();
        void Render();
        bool WindowShouldClose();
        void Exit();
    };
    void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
}
}
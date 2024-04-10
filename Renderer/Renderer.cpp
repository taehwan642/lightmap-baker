#include "Renderer.hpp"

void LightmapBaker::Renderer::Renderer::Initialize()
{
	 glfwInit();
     glfwWindow = glfwCreateWindow(640, 480, "Lightmap Baker", 0, 0);

     glfwMakeContextCurrent(glfwWindow);

     glfwSetKeyCallback(glfwWindow, KeyCallBack);
}

void LightmapBaker::Renderer::Renderer::Update()
{
    glfwPollEvents();
}

void LightmapBaker::Renderer::Renderer::Render()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(glfwWindow);
}

bool LightmapBaker::Renderer::Renderer::WindowShouldClose()
{
    return glfwWindowShouldClose(glfwWindow);
}

void LightmapBaker::Renderer::Renderer::Exit()
{
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void LightmapBaker::Renderer::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

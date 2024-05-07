#include <memory>
#include <cstdlib>
#include <ctime>
#include "GLEW/glew.h"
#include "Data/DataManager.hpp"
#include "Light/Light.hpp"
#include "Renderer/Renderer.hpp"

int main()
{
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    using namespace LightmapBaker;
    srand((unsigned int)time(NULL));

    std::shared_ptr<Data::DataManager> dataManager = std::make_shared<Data::DataManager>();
    auto meshList = dataManager->Load("Asset/tempModel.ttjj");
    std::cout << meshList.size() << std::endl;

    Renderer::Renderer& renderer = Renderer::Renderer::GetInstance();
    renderer.Initialize();

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        // Problem: glewInit failed, something is seriously wrong.
        std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    Light::RadiosityManager::GetInstance().Initialize();

    const int TARGET_FPS = 60;
    double lastTime = glfwGetTime();
    while (!renderer.WindowShouldClose()) 
    {
        renderer.Update();
        renderer.BeforeRender();
        renderer.Render();
        while (glfwGetTime() < lastTime + 1.0 / TARGET_FPS) {
            // Put the thread to sleep, yield, or simply do nothing
        }
        lastTime += 1.0 / TARGET_FPS;
    }
    renderer.Exit();

    return 0;
}
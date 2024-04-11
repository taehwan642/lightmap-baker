#include <memory>
#include "Data/DataManager.hpp"
#include "Light/Light.hpp"
#include "Renderer/Renderer.hpp"

int main()
{
    using namespace LightmapBaker;
    std::shared_ptr<Data::DataManager> dataManager = std::make_shared<Data::DataManager>();
    dataManager->Load("...");
    std::shared_ptr<Renderer::Renderer> renderer = std::make_shared<Renderer::Renderer>();
    renderer->Initialize();
    std::shared_ptr<Light::Radiosity> radiosity = std::make_shared<Light::Radiosity>();

    const int TARGET_FPS = 15;
    double lasttime = glfwGetTime();
    while (!renderer->WindowShouldClose()) 
    {
        renderer->Update();
        renderer->Render();
        while (glfwGetTime() < lasttime + 1.0 / TARGET_FPS) {
            // Put the thread to sleep, yield, or simply do nothing
        }
        lasttime += 1.0 / TARGET_FPS;
    }
    renderer->Exit();
    dataManager->Save("...");

    return 0;
}
#include <memory>
#include "GLEW/glew.h"
#include "Data/DataManager.hpp"
#include "Light/Light.hpp"
#include "Renderer/Renderer.hpp"

int main()
{
    using namespace LightmapBaker;
    std::shared_ptr<Data::DataManager> dataManager = std::make_shared<Data::DataManager>();
    auto meshList = dataManager->Load("Asset/tempModel.ttjj");
    std::cout << meshList.size() << std::endl;

    std::shared_ptr<Renderer::Renderer> renderer = std::make_shared<Renderer::Renderer>();
    renderer->Initialize();

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        // Problem: glewInit failed, something is seriously wrong.
        std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    /*auto preparedMeshList = dataManager->Load();
    for (int i = 0; i < preparedMeshList.size(); ++i)
    {
        renderer->AddRenderMesh(preparedMeshList[i]);
    }*/

    Light::RadiosityManager::GetInstance().SetRenderer(renderer);
    Light::RadiosityManager::GetInstance().Initialize();

    const int TARGET_FPS = 60;
    double lastTime = glfwGetTime();
    while (!renderer->WindowShouldClose()) 
    {
        Light::RadiosityManager::GetInstance().Update();
        renderer->Update();
        renderer->Render();
        while (glfwGetTime() < lastTime + 1.0 / TARGET_FPS) {
            // Put the thread to sleep, yield, or simply do nothing
        }
        lastTime += 1.0 / TARGET_FPS;
    }
    renderer->Exit();
    dataManager->Save("...");

    return 0;
}
#include <memory>
#include <cstdlib>
#include <ctime>
#include "GLEW/glew.h"
#include "Data/DataManager.hpp"
#include "Light/Light.hpp"
#include "Renderer/Renderer.hpp"

int main()
{
    using namespace LightmapBaker;
    srand((unsigned int)time(NULL));

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
        renderer->Update();
        renderer->BeforeRender();
        Light::RadiosityManager::GetInstance().Update();
        renderer->Render();
        while (glfwGetTime() < lastTime + 1.0 / TARGET_FPS) {
            // Put the thread to sleep, yield, or simply do nothing
        }
        lastTime += 1.0 / TARGET_FPS;
    }
    renderer->Exit();
    std::vector<UINT8> pngData;
    pngData.resize(100 * 100 * 3); // R G B
    for (int i = 0; i < 100 * 100 * 3; ++i) pngData[i] = 255;
    dataManager->Save("lightmap.png", 100, 100, pngData.data());

    return 0;
}
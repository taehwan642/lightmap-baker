#include <memory>
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
    std::shared_ptr<Light::Radiosity> radiosity = std::make_shared<Light::Radiosity>();
    while (!renderer->WindowShouldClose()) 
    {
        renderer->Update();
        renderer->Render();
    }
    renderer->Exit();
    dataManager->Save("...");

    return 0;
}
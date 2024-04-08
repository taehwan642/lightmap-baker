#include <memory>

#include "Data/DataManager.hpp"
#include "Light/Light.hpp"
#include "Renderer/Renderer.hpp"

int main() {
  using namespace LightmapBaker;
  std::shared_ptr<Data::DataManager> dataManager =
      std::make_shared<Data::DataManager>();

  dataManager->Load("...");

  std::shared_ptr<Renderer::Renderer> renderer =
      std::make_shared<Renderer::Renderer>();

  renderer->Initialize();
  renderer->Update();

  std::shared_ptr<Light::Radiosity> radiosity =
      std::make_shared<Light::Radiosity>();

  renderer->Render();
  renderer->Exit();
  dataManager->Save("...");

  return 0;
}
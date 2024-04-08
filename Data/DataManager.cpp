#include "DataManager.hpp"

#include <iostream>

void LightmapBaker::Data::DataManager::Save(std::string path) {
  std::cout << "Save "
            << " " << path << std::endl;
}

void LightmapBaker::Data::DataManager::Load(std::string path) {
  std::cout << "Load "
            << " " << path << std::endl;
}

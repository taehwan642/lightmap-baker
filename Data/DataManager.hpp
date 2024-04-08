#pragma once
#include <string>

namespace LightmapBaker {
namespace Data {
class DataManager {
 private:
 public:
  void Save(std::string path);
  void Load(std::string path);
};
}  // namespace Data
}  // namespace LightmapBaker

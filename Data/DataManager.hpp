#pragma once
#include <string>
#include <list>
#include "../Renderer/Mesh.hpp"

namespace LightmapBaker
{
namespace Data
{
    class DataManager
    {
    private:
    public:
        void Save(std::string path);
        std::list<std::shared_ptr<Renderer::Mesh>> Load(const std::string& path);
    };
}
}

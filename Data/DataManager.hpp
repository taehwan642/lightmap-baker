﻿#pragma once
#include <string>
#include <vector>
#include <memory>
#include "GLM/vec3.hpp"
#include "../Renderer/Mesh.hpp"
#include "../Light/Lightmap.hpp"

namespace LightmapBaker
{
namespace Data
{
    class DataManager
    {
    private:
        void RasterTriangle(Thekla::Atlas_Output_Vertex p1, Thekla::Atlas_Output_Vertex p2, Thekla::Atlas_Output_Vertex p3, glm::u8vec3 color, std::vector<glm::u8vec3>& data, int width);
    public:
        bool Save(const std::string& path, const std::shared_ptr<Light::Lightmap>& lightMap);
        bool Save(std::string path, int width, int height, const void* data);
        std::vector<std::shared_ptr<Renderer::Mesh>> Load(const std::string& path);
        // for debug
        std::vector<std::shared_ptr<Renderer::Mesh>> Load();
        std::vector<Renderer::Vertex> GetVertices();
    };
}
}

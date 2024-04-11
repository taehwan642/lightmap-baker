#pragma once
#include <vector>
#include "Renderer.hpp"
#include "GLM/vec3.hpp"

namespace LightmapBaker
{
namespace Renderer
{
	class Mesh
	{
    public:
        std::vector<int> indices;
        glm::vec3 normal;
        int patchLevel;
        int elementLevel;
        float area;
        glm::vec3 reflectance;
        glm::vec3 emission;

    public:
        void Initialize();
        void Render();
        void Destroy();
	};
}
}

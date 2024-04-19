#pragma once
#include <vector>
#include "GLM/vec3.hpp"
#include "GLFW/glfw3.h"

namespace LightmapBaker
{
namespace Renderer
{
	class Mesh
	{
    public:
        std::vector<glm::vec3> vertices;
        std::vector<GLubyte> indices;
        glm::vec3 normal;
        int patchLevel;
        int elementLevel;
        float area;
        glm::vec3 reflectance;
        glm::vec3 emission;

        glm::vec3 color;

    public:
        Mesh();
        Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLubyte>& indices, const glm::vec3& normal, const int& patchLevel, const int& elementLevel, const float& area, const glm::vec3& reflectance, const glm::vec3& emission);
        void Initialize();
        void Render();
        void Destroy();
        glm::vec3 GetVertexByIndex(int index);
	};
}
}

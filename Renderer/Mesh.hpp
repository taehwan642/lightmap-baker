#pragma once
#include <vector>
#include "GLM/vec3.hpp"
#include "GLM/vec2.hpp"
#include "GLFW/glfw3.h"

namespace LightmapBaker
{
namespace Renderer
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
    };

	class Mesh
	{
    private:
        GLuint ibo;
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        glm::vec3 normal;
        int patchLevel;
        int elementLevel;
        float area;
        glm::vec3 reflectance;
        glm::vec3 emission;
        glm::vec3 color;

    public:
        Mesh();
        Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const glm::vec3& normal, const int& patchLevel, const int& elementLevel, const float& area, const glm::vec3& reflectance, const glm::vec3& emission);
        Mesh(const Mesh& mesh);
        void Initialize();
        void Render();
        void Destroy();
        Vertex GetVertexByIndex(int index);
        void CreateIndexBuffer();
	};
}
}

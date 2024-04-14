#include "Mesh.hpp"

LightmapBaker::Renderer::Mesh::Mesh()
{
	normal = glm::vec3(0, 0, 0);
	patchLevel = 0;
	elementLevel = 0;
	area = 0.0f;
	reflectance = glm::vec3(0, 0, 0);
	emission = glm::vec3(0, 0, 0);
}

LightmapBaker::Renderer::Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<int>& indices, const glm::vec3& normal, const int& patchLevel, const int& elementLevel, const float& area, const glm::vec3& reflectance, const glm::vec3& emission)
LightmapBaker::Renderer::Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLubyte>& indices, const glm::vec3& normal, const int& patchLevel, const int& elementLevel, const float& area, const glm::vec3& reflectance, const glm::vec3& emission)
{
	this->vertices = vertices;
	this->indices = indices;
	this->normal = normal;
	this->patchLevel = patchLevel;
	this->elementLevel = elementLevel;
	this->area = area;
	this->reflectance = reflectance;
	this->emission = emission;
}

void LightmapBaker::Renderer::Mesh::Initialize()
{
}

void LightmapBaker::Renderer::Mesh::Render()
{
}

void LightmapBaker::Renderer::Mesh::Destroy()
{
}

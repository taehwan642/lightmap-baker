#include "GLEW/glew.h"
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

LightmapBaker::Renderer::Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices, const glm::vec3& normal, const int& patchLevel, const int& elementLevel, const float& area, const glm::vec3& reflectance, const glm::vec3& emission)
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
	glColor3f(reflectance.r, reflectance.g, reflectance.b);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void LightmapBaker::Renderer::Mesh::Destroy()
{
}

glm::vec3 LightmapBaker::Renderer::Mesh::GetVertexByIndex(int index)
{
	return vertices[index];
}

void LightmapBaker::Renderer::Mesh::CreateIndexBuffer()
{
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
}

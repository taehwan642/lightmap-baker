#include "GLEW/glew.h"
#include "Mesh.hpp"

LightmapBaker::Renderer::Mesh::Mesh()
{
	patchLevel = 0;
	elementLevel = 0;
	area = 0.0f;
	reflectance = glm::vec3(0, 0, 0);
	emission = glm::vec3(0, 0, 0);
	color = glm::vec3(0, 0, 0);
}

LightmapBaker::Renderer::Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const glm::vec3& normal, const int& patchLevel, const int& elementLevel, const float& area, const glm::vec3& reflectance, const glm::vec3& emission)
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

LightmapBaker::Renderer::Mesh::Mesh(const Mesh& mesh)
{
	vertices = mesh.vertices;
	indices = mesh.indices;
	normal = mesh.normal;
	patchLevel = mesh.patchLevel;
	elementLevel = mesh.elementLevel;
	area = mesh.area;
	reflectance = mesh.reflectance;
	emission = mesh.emission;
	color = mesh.color;
}

void LightmapBaker::Renderer::Mesh::Initialize()
{
}

void LightmapBaker::Renderer::Mesh::Render(RenderOption option)
{
	if (texture != 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	glColor3f(color.r, color.g, color.b);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].uv);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glDrawElements(option.drawMode, indices.size(), GL_UNSIGNED_INT, nullptr);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (texture != 0)
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void LightmapBaker::Renderer::Mesh::Destroy()
{
}

LightmapBaker::Renderer::Vertex LightmapBaker::Renderer::Mesh::GetVertexByIndex(int index)
{
	return vertices[index];
}

void LightmapBaker::Renderer::Mesh::CreateIndexBuffer()
{
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
}

#pragma once
#include <memory>
#include <vector>
#include "thekla/thekla_atlas.h"
#include "GLM/vec3.hpp"
#include "../Renderer/Mesh.hpp"

namespace LightmapBaker
{
namespace Light
{
	class Lightmap
	{
	private:
		void GetInputMesh(const std::vector<std::shared_ptr<Renderer::Mesh>>& meshList, Thekla::Atlas_Input_Mesh& inputMesh);
		std::vector<std::shared_ptr<Renderer::Mesh>> bakedMeshList;
	public:
		std::vector<glm::vec3> vertexColors;
		std::vector<Renderer::Vertex> meshVertices;
		Thekla::Atlas_Output_Mesh* outputMesh;
		void Bake(const std::shared_ptr<Renderer::Mesh>& mesh);
		void Bake(const std::vector<std::shared_ptr<Renderer::Mesh>>& meshList);
		std::vector<std::shared_ptr<Renderer::Mesh>> GetAtlasUVMesh();
		void Destroy();
	};
}
}
#pragma once
#include <memory>
#include "thekla/thekla_atlas.h"
#include "../Renderer/Mesh.hpp"

namespace LightmapBaker
{
namespace Light
{
	class Lightmap
	{
	private:
		void GetInputMesh(const std::vector<std::shared_ptr<Renderer::Mesh>>& meshList, Thekla::Atlas_Input_Mesh& inputMesh);

	public:
		Thekla::Atlas_Output_Mesh* outputMesh;
		void Bake(const std::shared_ptr<Renderer::Mesh>& mesh);
		void Bake(const std::vector<std::shared_ptr<Renderer::Mesh>>& meshList);
		void Destroy();
	};
}
}
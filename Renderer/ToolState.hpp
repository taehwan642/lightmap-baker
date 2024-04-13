#pragma once
#include <string>

namespace LightmapBaker
{
namespace Renderer
{
	enum class ToolStateEnum
	{
		BEFORE_RADIOSITY_CALCULATION,
		PROGRESS_RADIOSITY_CALCULATION,
		BEFORE_LIGHTMAP_BAKE,
		PROGRESS_LIGHTMAP_BAKE,
		AFTER_LIGHTMAP_BAKE
	};

	class ToolState
	{
	private:
		ToolStateEnum currentState = ToolStateEnum::BEFORE_RADIOSITY_CALCULATION;
		void RenderBeforeRadiosityCalculationUI();
		void RenderProgressRadiosityCalculationUI();
		void RenderBeforeLightmapBakeUI();
		void RenderProgressLightmapBakeUI();
		void RenderAfterLightmapBakeUI();

	private:
		float radiosityCalculationProgressValue = 0.0f;
		float lightmapBakeProgressValue = 0.0f;
		int lightMapQuality = 1;
		void ProgressUI(const float& progress, const std::string& text);
		void CompareUI();

	public:
		void UpdateCurrentState(const ToolStateEnum& state);
		void Update();
		void RenderCurrentUI();
	};
}
}
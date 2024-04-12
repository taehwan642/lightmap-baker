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
		float progressValue = 0.7f;
		void ProgressUI(float& _value, std::string _text);
		void AfterProgressButtonUI(std::string _text);

	public:
		void UpdateCurrentState(const ToolStateEnum& state);
		void RenderCurrentUI();
	};
}
}
#pragma once
#include "GLUT/glut.h"
#include <string>
#include <list>

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
		std::list<double> frames;

		void RenderBeforeRadiosityCalculationUI();
		void RenderProgressRadiosityCalculationUI();
		void RenderBeforeLightmapBakeUI();
		void RenderProgressLightmapBakeUI();
		void RenderAfterLightmapBakeUI();

	private:
		void RenderHemicubeRenderedImage();

	private:
		int lightMapQuality = 1;
		float compareXPosition = 0.0f;

		void ProgressUI(const std::string& text);
		void CompareUI();

	public:
		void UpdateCurrentState(const ToolStateEnum& state);
		void Update();
		void RenderCurrentUI();
		void SetFrame(double frame);
	};
}
}
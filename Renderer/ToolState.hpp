#pragma once
#include "GLUT/glut.h"
#include <string>
#include <list>
#include <vector>
#include <memory>
#include "UI.hpp"
#include "../Light/Lightmap.hpp"

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

	enum class ThreadState
	{
		IDLE,
		RUNNING,
		DONE
	};

	class ToolState
	{
	private:
		ToolStateEnum currentState = ToolStateEnum::BEFORE_RADIOSITY_CALCULATION;
		std::list<double> frames;
		std::vector<std::shared_ptr<UI>> uis;
		std::shared_ptr<UI> renderingUI;
		ThreadState threadState = ThreadState::IDLE;
		std::shared_ptr<Light::Lightmap> threadLightmap;

		void RenderCompareModel(std::shared_ptr<CompareUI> compareUI);
	public:
		void Initialize();
		void UpdateCurrentState(const ToolStateEnum& state);
		void Update();
		void RenderCurrentUI();
		void SetFrame(double frame);
		float GetCompareXPosition();
		bool GetIsDrawMeshLine();
	};
}
}
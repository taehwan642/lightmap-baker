#pragma once
#include "GLUT/glut.h"
#include <string>
#include <vector>
#include <memory>
#include "UI.hpp"
#include "../Light/Lightmap.hpp"
#include "../Utility/Singleton.hpp"
#include "RenderEnums.hpp"

namespace LightmapBaker
{
namespace Renderer
{
	class ToolState : public Utility::Singleton<ToolState>
	{
	private:
		~ToolState() = default;
		friend class Utility::Singleton<ToolState>;

	private:
		ToolStateEnum currentState = ToolStateEnum::BEFORE_RADIOSITY_CALCULATION;
		std::vector<std::shared_ptr<UI>> uis;
		std::shared_ptr<UI> renderingUI;
		std::shared_ptr<FrameUI> frameUI;
		std::shared_ptr<LoggerUI> loggerUI;
		ThreadState threadState = ThreadState::IDLE;
		std::shared_ptr<Light::Lightmap> threadLightmap;

	public:
		ToolState();

		void UpdateCurrentState(const ToolStateEnum& state);
		void Update();
		void RenderCurrentUI();
		float GetSplitXPosition();
		bool GetIsDrawMeshLine();
		GLenum GetMeshDrawMode();
	};
}
}
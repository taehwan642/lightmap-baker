#include "ToolState.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

void LightmapBaker::Renderer::ToolState::RenderBeforeRadiosityCalculationUI()
{
	ImGui::Begin("Controller");
	if (ImGui::Button("Calculate Radiosity"))
	{
	}
	ImGui::End();
}

void LightmapBaker::Renderer::ToolState::RenderProgressRadiosityCalculationUI()
{
}

void LightmapBaker::Renderer::ToolState::RenderBeforeLightmapBakeUI()
{
}

void LightmapBaker::Renderer::ToolState::RenderProgressLightmapBakeUI()
{
}

void LightmapBaker::Renderer::ToolState::RenderAfterLightmapBakeUI()
{
}

void LightmapBaker::Renderer::ToolState::UpdateCurrentState(const ToolStateEnum& state)
{
	currentState = state;
}

void LightmapBaker::Renderer::ToolState::RenderCurrentUI()
{
	switch (currentState)
	{
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_RADIOSITY_CALCULATION:
		RenderBeforeRadiosityCalculationUI();
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION:
		RenderProgressRadiosityCalculationUI();
		break;
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_LIGHTMAP_BAKE:
		RenderBeforeLightmapBakeUI();
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_LIGHTMAP_BAKE:
		RenderProgressLightmapBakeUI();
		break;
	case LightmapBaker::Renderer::ToolStateEnum::AFTER_LIGHTMAP_BAKE:
		RenderAfterLightmapBakeUI();
		break;
	default:
		break;
	}
}

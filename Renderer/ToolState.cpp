#include "ToolState.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>

void LightmapBaker::Renderer::ToolState::RenderBeforeRadiosityCalculationUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 155, ImGui::GetMainViewport()->Size.y - 33));
	ImGui::Begin("Controller", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Calculate Radiosity", ImVec2(141.0f, 19.0f)))
	{
	}
	ImGui::End();
}

void LightmapBaker::Renderer::ToolState::RenderProgressRadiosityCalculationUI()
{
	ProgressUI(progressValue, "   Radiosity calculation in progress...");
}

void LightmapBaker::Renderer::ToolState::RenderBeforeLightmapBakeUI()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 360, ImGui::GetMainViewport()->Size.y - 42));
	ImGui::Begin("Controller4", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text("LightingMap Quality : 1");
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 190, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("Controller5", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::ArrowButton("Up", ImGuiDir_Up))
	{
	}

	if (ImGui::ArrowButton("Down", ImGuiDir_Down))
	{
	}
	ImGui::End();

	AfterProgressButtonUI("Bake");
}

void LightmapBaker::Renderer::ToolState::RenderProgressLightmapBakeUI()
{
	ProgressUI(progressValue, "          Baking in progress...");
}

void LightmapBaker::Renderer::ToolState::RenderAfterLightmapBakeUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 156, ImGui::GetMainViewport()->Size.y - 80));
	ImGui::Begin("Controller4", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	const char* items[] = { "Basic", "Radiosity" };
	static const char* curItem = NULL;

	ImGui::BeginCombo("", curItem);
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("");
	for (int i = 0; i < IM_ARRAYSIZE(items); i++)
	{
		bool is_selected = (curItem == items[i]);
		if (ImGui::Selectable(items[i], is_selected, ImGuiSelectableFlags_None))
			curItem = items[i];
		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}
	ImGui::EndCombo();
	ImGui::End();

	AfterProgressButtonUI("New Load");
}

void LightmapBaker::Renderer::ToolState::ProgressUI(float& _value, std::string _text)
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("Controller", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::ProgressBar(_value);
	ImGui::Text(_text.c_str());
	ImGui::End();
}

void LightmapBaker::Renderer::ToolState::AfterProgressButtonUI(std::string _text)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 155, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("Controller1", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Compare", ImVec2(141.0f, 19.0f)))
	{
	}

	if (ImGui::Button(_text.c_str(), ImVec2(141.0f, 19.0f)))
	{
	}
	ImGui::End();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 2.5f, -20.0f));
	ImGui::Begin("Controller2", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("", ImVec2(5.0f, 1180.0f)))
	{
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 20, (ImGui::GetMainViewport()->Size.y / 2.0f) - 15));
	ImGui::Begin("Controller3", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::ArrowButton("Left", ImGuiDir_Left))
	{
	}
	ImGui::End();
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

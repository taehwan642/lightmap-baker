#include "UI.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void LightmapBaker::Renderer::ProgressUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - (36.0f * (ImGui::GetWindowHeight() / 480.0f) + 19.0f)));
	ImGui::Begin("Progress", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text(text.c_str());
	ImGui::End();
}

void LightmapBaker::Renderer::CompareUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - (19.0f * (ImGui::GetWindowHeight() / 480.0f) + 36.0f)));
	ImGui::Begin("Compare Helper", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text("Move splitter by left / right arrow");
	ImGui::End();

	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)))
	{
		floatData -= ImGui::GetIO().DeltaTime * 1000.0f;
		if (floatData < 0.0f) floatData = 0.0f;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow)))
	{
		floatData += ImGui::GetIO().DeltaTime * 1000.0f;
		if (ImGui::GetMainViewport()->Size.x < floatData) floatData = ImGui::GetMainViewport()->Size.x;
	}
}

void LightmapBaker::Renderer::BeforeRadiosityCalculationUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * (ImGui::GetWindowWidth() / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (19.0f * (ImGui::GetWindowHeight() / 480.0f) + 14.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("CalculateRadiosity", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Calculate Radiosity", ImVec2(141.0f * (ImGui::GetWindowWidth() / 640.0f), 19.0f * (ImGui::GetWindowHeight() / 480.0f))))
	{
		callbacks["CalculateRadiosityButton"]();
	}
	ImGui::End();
}

void LightmapBaker::Renderer::ProgressRadiosityCalculationUI::RenderUI()
{
	text = "Radiosity calculation in progress";
	ProgressUI::RenderUI();

	ImVec2 resolution = ImVec2(resolutionX, resolutionY);
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - resolution.y));
	ImGui::SetNextWindowSize(resolution, ImGuiCond_Once);
	ImGui::Begin("Texture", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	ImGui::Image((ImTextureID)renderTexture, resolution);
	ImGui::End();
}

void LightmapBaker::Renderer::BeforeLightmapBakeUI::RenderUI()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141 * (ImGui::GetWindowWidth() / 640.0f) + 205.0f), ImGui::GetMainViewport()->Size.y - (19.0f * (ImGui::GetWindowHeight() / 480.0f) + 23.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * (ImGui::GetWindowWidth() / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (38.0f * (ImGui::GetWindowHeight() / 480.0f) + 17.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("BakeBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	if (ImGui::Button("Bake", ImVec2(141.0f * (ImGui::GetWindowWidth() / 640.0f), 19.0f * (ImGui::GetWindowHeight() / 480.0f))))
	{
		callbacks["BakeLightmapButton"]();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	CompareUI::RenderUI();
}

void LightmapBaker::Renderer::ProgressLightmapBakeUI::RenderUI()
{
	text = "Lightmap baking in progress";
	ProgressUI::RenderUI();
}

void LightmapBaker::Renderer::AfterLightmapBakeUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - (5.0f * (ImGui::GetWindowHeight() / 480.0f) + 25.0f)));
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);
	ImGui::Begin("BasicRadiosityCombo", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	const char* items[] = { "Basic", "Radiosity" };
	static const char* curItem = items[0];

	if (ImGui::BeginCombo("Compare with", curItem))
	{
		for (int i = 0; i < IM_ARRAYSIZE(items); i++)
		{
			bool is_selected = (curItem == items[i]);
			if (ImGui::Selectable(items[i], is_selected, ImGuiSelectableFlags_None))
			{
				curItem = items[i];
				integerData = i;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * (ImGui::GetWindowWidth() / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (38.0f * (ImGui::GetWindowHeight() / 480.0f) + 17.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("NewLoadBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("New Load", ImVec2(141.0f * (ImGui::GetWindowWidth() / 640.0f), 19.0f * (ImGui::GetWindowHeight() / 480.0f))))
	{
		callbacks["NewLoadButton"]();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	CompareUI::RenderUI();
}

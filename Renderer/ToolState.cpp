#include "ToolState.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "../Light/Light.hpp"
#include "../Data/DataManager.hpp"
#include "../Light/Lightmap.hpp"

void LightmapBaker::Renderer::ToolState::RenderBeforeRadiosityCalculationUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 155, ImGui::GetMainViewport()->Size.y - 33));
	ImGui::Begin("CalculateRadiosity", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Calculate Radiosity", ImVec2(141.0f, 19.0f)))
	{
		UpdateCurrentState(ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION);
	}
	ImGui::End();
}

void LightmapBaker::Renderer::ToolState::RenderProgressRadiosityCalculationUI()
{
	ProgressUI("Radiosity calculation in progress");
}

void LightmapBaker::Renderer::ToolState::RenderBeforeLightmapBakeUI()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 360, ImGui::GetMainViewport()->Size.y - 42));
	ImGui::Begin("LightMapQuality", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	std::string qualityString = "Lightmap Quality : ";
	ImGui::Text((qualityString + std::to_string(lightMapQuality)).c_str());
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 190, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("UpDownBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::ArrowButton("Up", ImGuiDir_Up))
	{
		++lightMapQuality;
	}

	if (ImGui::ArrowButton("Down", ImGuiDir_Down))
	{
		--lightMapQuality;
		if (lightMapQuality < 0) lightMapQuality = 0;
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 155, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("CompareBakeBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Compare", ImVec2(141.0f, 19.0f)))
	{
	}

	if (ImGui::Button("Bake", ImVec2(141.0f, 19.0f)))
	{
		UpdateCurrentState(ToolStateEnum::PROGRESS_LIGHTMAP_BAKE);
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	CompareUI();
}

void LightmapBaker::Renderer::ToolState::RenderProgressLightmapBakeUI()
{
	ProgressUI("Lightmap baking in progress");
}

void LightmapBaker::Renderer::ToolState::RenderAfterLightmapBakeUI()
{
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - 30));
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
				curItem = items[i];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::End();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 155, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("CompareNewLoadBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Compare", ImVec2(141.0f, 19.0f)))
	{
	}

	if (ImGui::Button("New Load", ImVec2(141.0f, 19.0f)))
	{
		Light::RadiosityManager::GetInstance().Destroy();
		Light::RadiosityManager::GetInstance().Initialize();
		UpdateCurrentState(ToolStateEnum::BEFORE_RADIOSITY_CALCULATION);
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	CompareUI();
}

void LightmapBaker::Renderer::ToolState::RenderHemicubeRenderedImage()
{
	ImVec2 resolution = ImVec2(Light::RadiosityManager::GetInstance().hemiCubeRenderTarget.resolution.x, Light::RadiosityManager::GetInstance().hemiCubeRenderTarget.resolution.y);
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - resolution.y));
	ImGui::SetNextWindowSize(resolution, ImGuiCond_Once);
	ImGui::Begin("Texture", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	ImGui::Image((ImTextureID)Light::RadiosityManager::GetInstance().hemiCubeRenderTarget.renderTexture, resolution);
	ImGui::End();
}

void LightmapBaker::Renderer::ToolState::ProgressUI(const std::string& text)
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("Progress", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text(text.c_str());
	ImGui::End();
}

void LightmapBaker::Renderer::ToolState::CompareUI()
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - 55));
	ImGui::Begin("Compare Helper", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text("Move splitter by left / right arrow");
	ImGui::End();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)))
	{
		compareXPosition -= ImGui::GetIO().DeltaTime * 1000.0f;
		if (compareXPosition < 0.0f) compareXPosition = 0.0f;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow)))
	{
		compareXPosition += ImGui::GetIO().DeltaTime * 1000.0f;
		if (ImGui::GetMainViewport()->Size.x - 25.0f < compareXPosition) compareXPosition = ImGui::GetMainViewport()->Size.x - 25.0f;
	}
	ImGui::SetNextWindowPos(ImVec2(compareXPosition, -20.0f));
	ImGui::SetNextWindowSize(ImVec2(100, ImGui::GetMainViewport()->Size.y + 100));
	ImGui::Begin("MiddleLine", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("##", ImVec2(5.0f, 1180.0f)))
	{
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
}

void LightmapBaker::Renderer::ToolState::UpdateCurrentState(const ToolStateEnum& state)
{
	currentState = state;
}

void LightmapBaker::Renderer::ToolState::Update()
{
	switch (currentState)
	{
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_RADIOSITY_CALCULATION:
	{
		// Model Import
	}
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION:
	{
		// Calc Radiosity
		if (!Light::RadiosityManager::GetInstance().Update())
		{
			compareXPosition = (ImGui::GetMainViewport()->Size.x / 2.0f) - 2.5f;
			UpdateCurrentState(ToolStateEnum::BEFORE_LIGHTMAP_BAKE);
		}
	}
		break;
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_LIGHTMAP_BAKE:
	{

	}
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_LIGHTMAP_BAKE:
	{
		std::shared_ptr<Light::Lightmap> lightMap = std::make_shared<Light::Lightmap>();
		std::vector<std::shared_ptr<Mesh>> meshList;
		for (int i = 0; i < Light::RadiosityManager::GetInstance().elements.size(); ++i)
		{
			meshList.push_back(Light::RadiosityManager::GetInstance().elements[i]->mesh);
		}
		lightMap->Bake(meshList);
		std::shared_ptr<Data::DataManager> dataManager = std::make_shared<Data::DataManager>();
		if (!dataManager->Save("lightmap.png", lightMap))
			std::cout << "Bake Error" << std::endl;
		lightMap->Destroy();

		compareXPosition = (ImGui::GetMainViewport()->Size.x / 2.0f) - 2.5f;
		UpdateCurrentState(ToolStateEnum::AFTER_LIGHTMAP_BAKE);
	}
		break;
	case LightmapBaker::Renderer::ToolStateEnum::AFTER_LIGHTMAP_BAKE:
		break;
	default:
		break;
	}
}

void LightmapBaker::Renderer::ToolState::RenderCurrentUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 255, 33));
	ImGui::SetNextWindowSize(ImVec2(200, 50), ImGuiCond_Once);
	ImGui::Begin("Frames", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	while (frames.size() > 100) frames.pop_front();

	std::vector<float> framesVector;
	for (const auto& value : frames)
	{
		framesVector.push_back((float)value);
	}

	ImGui::PlotLines("Frames", framesVector.data(), framesVector.size());
	ImGui::End();


	switch (currentState)
	{
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_RADIOSITY_CALCULATION:
		RenderBeforeRadiosityCalculationUI();
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION:
	{
		RenderProgressRadiosityCalculationUI();
		RenderHemicubeRenderedImage();
	}
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

void LightmapBaker::Renderer::ToolState::SetFrame(double frame)
{
	frames.push_back(frame);
}

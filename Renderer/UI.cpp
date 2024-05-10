#include "UI.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>

int LightmapBaker::Renderer::UI::screenWidth = 0;
int LightmapBaker::Renderer::UI::screenHeight = 0;

void LightmapBaker::Renderer::ProgressUI::InitializeUI()
{

}

void LightmapBaker::Renderer::ProgressUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - (36.0f * ((float)screenHeight / 480.0f) + 19.0f)));
	ImGui::Begin("Progress", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text(text.c_str());
	ImGui::End();
}

void LightmapBaker::Renderer::CompareUI::InitializeUI()
{
	comparePositionX = (ImGui::GetMainViewport()->Size.x / 2.0f);
}

void LightmapBaker::Renderer::CompareUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - (19.0f * ((float)screenHeight / 480.0f) + 36.0f)));
	ImGui::Begin("Compare Helper", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text("Move splitter by left / right arrow");
	ImGui::Checkbox("Draw mesh line", &isDrawMeshLine);
	ImGui::End();

	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)))
	{
		comparePositionX -= ImGui::GetIO().DeltaTime * 1000.0f;
		if (comparePositionX < 0.0f) comparePositionX = 0.0f;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow)))
	{
		comparePositionX += ImGui::GetIO().DeltaTime * 1000.0f;
		if (ImGui::GetMainViewport()->Size.x < comparePositionX) comparePositionX = ImGui::GetMainViewport()->Size.x;
	}
}

void LightmapBaker::Renderer::BeforeRadiosityCalculationUI::InitializeUI()
{
}

void LightmapBaker::Renderer::BeforeRadiosityCalculationUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * ((float)screenWidth / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (19.0f * ((float)screenHeight / 480.0f) + 14.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("CalculateRadiosity", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Calculate Radiosity", ImVec2(141.0f * ((float)screenWidth / 640.0f), 19.0f * ((float)screenHeight / 480.0f))))
	{
		callbacks["CalculateRadiosityButton"]();
	}
	ImGui::End();
}

void LightmapBaker::Renderer::ProgressRadiosityCalculationUI::InitializeUI()
{
	ProgressUI::InitializeUI();
	text = "Radiosity calculation in progress";
}

void LightmapBaker::Renderer::ProgressRadiosityCalculationUI::RenderUI()
{
	ProgressUI::RenderUI();

	ImVec2 resolution = ImVec2(resolutionX, resolutionY);
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - resolution.y));
	ImGui::SetNextWindowSize(resolution, ImGuiCond_Once);
	ImGui::Begin("Texture", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	ImGui::Image((ImTextureID)renderTexture, resolution);
	ImGui::End();
}

void LightmapBaker::Renderer::BeforeLightmapBakeUI::InitializeUI()
{
	CompareUI::InitializeUI();
}

void LightmapBaker::Renderer::BeforeLightmapBakeUI::RenderUI()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141 * ((float)screenWidth / 640.0f) + 205.0f), ImGui::GetMainViewport()->Size.y - (19.0f * ((float)screenHeight / 480.0f) + 23.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * ((float)screenWidth / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (38.0f * ((float)screenHeight / 480.0f) + 17.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("BakeBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	if (ImGui::Button("Bake", ImVec2(141.0f * ((float)screenWidth / 640.0f), 19.0f * ((float)screenHeight / 480.0f))))
	{
		callbacks["BakeLightmapButton"]();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	CompareUI::RenderUI();
}

void LightmapBaker::Renderer::ProgressLightmapBakeUI::InitializeUI()
{
	ProgressUI::InitializeUI();
	text = "Lightmap baking in progress";
}

void LightmapBaker::Renderer::ProgressLightmapBakeUI::RenderUI()
{
	ProgressUI::RenderUI();
}

void LightmapBaker::Renderer::AfterLightmapBakeUI::InitializeUI()
{
	CompareUI::InitializeUI();
}

void LightmapBaker::Renderer::AfterLightmapBakeUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - (5.0f * ((float)screenHeight / 480.0f) + 25.0f)));
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
				compareIndex = i;
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

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * ((float)screenWidth / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (38.0f * ((float)screenHeight / 480.0f) + 17.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("NewLoadBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("New Load", ImVec2(141.0f * ((float)screenWidth / 640.0f), 19.0f * ((float)screenHeight / 480.0f))))
	{
		callbacks["NewLoadButton"]();
	}
	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	CompareUI::RenderUI();
}

void LightmapBaker::Renderer::FrameUI::AddFPS(double fps)
{
	fpsList.push_back(fps);
}

void LightmapBaker::Renderer::FrameUI::InitializeUI()
{
	fpsList.clear();
}

void LightmapBaker::Renderer::FrameUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - 255, 33));
	ImGui::SetNextWindowSize(ImVec2(200, 50), ImGuiCond_Once);
	ImGui::Begin("Frames", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	while (fpsList.size() > 100) fpsList.pop_front();

	std::vector<float> framesVector;
	for (const auto& value : fpsList)
	{
		framesVector.push_back((float)value);
	}

	ImGui::PlotLines("Frames", framesVector.data(), framesVector.size());
	ImGui::End();
}

std::list<std::string> LightmapBaker::Renderer::LoggerUI::logs = std::list<std::string>();

void LightmapBaker::Renderer::LoggerUI::AddLog(const std::string& log)
{
	logs.push_back(log);
}

void LightmapBaker::Renderer::LoggerUI::InitializeUI()
{
}

void LightmapBaker::Renderer::LoggerUI::RenderUI()
{
	ImGui::SetNextWindowPos(ImVec2(100, 33));
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_Once);
	ImGui::Begin("Logger", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);

	if (isEnabled == true)
	{
		ImGui::BeginChild("LogScroll");
		for (auto& iter : logs)
		{
			ImGui::Text(iter.c_str());
		}
		ImGui::EndChild();

		if (ImGui::Button("Close", ImVec2(200.0f * ((float)screenWidth / 640.0f), 200.0f * ((float)screenHeight / 480.0f))))
		{
			isEnabled = false;
		}

		if (ImGui::Button("Clear", ImVec2(400.0f * ((float)screenWidth / 640.0f), 200.0f * ((float)screenHeight / 480.0f))))
		{
			logs.clear();
		}
	}
	else
	{
		if (ImGui::Button("Enable", ImVec2(400.0f * ((float)screenWidth / 640.0f), 200.0f * ((float)screenHeight / 480.0f))))
		{
			isEnabled = true;
		}
	}

	ImGui::End();
}

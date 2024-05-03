#include "ToolState.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include "../Light/Light.hpp"
#include "../Data/DataManager.hpp"
#include "../Light/Lightmap.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void LightmapBaker::Renderer::ToolState::RenderBeforeRadiosityCalculationUI()
{
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * ((float)curScreenWidth / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (19.0f * ((float)curScreenHeight / 480.0f) + 14.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("CalculateRadiosity", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Calculate Radiosity", ImVec2(141.0f * ((float)curScreenWidth / 640.0f), 19.0f * ((float)curScreenHeight / 480.0f))))
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
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141 * ((float)curScreenWidth / 640.0f) + 205.0f), ImGui::GetMainViewport()->Size.y - (19.0f * ((float)curScreenHeight / 480.0f) + 23.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("LightMapQuality", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	std::string qualityString = "Lightmap Quality : ";
	ImGui::Text((qualityString + std::to_string(lightMapQuality)).c_str());
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * ((float)curScreenWidth / 640.0f) + 49.0f), ImGui::GetMainViewport()->Size.y - (19.0f * ((float)curScreenHeight / 480.0f) + 36.0f)));
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

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * ((float)curScreenWidth / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (38.0f * ((float)curScreenHeight / 480.0f) + 17.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("CompareBakeBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Compare", ImVec2(141.0f * ((float)curScreenWidth / 640.0f), 19.0f * ((float)curScreenHeight / 480.0f))))
	{
	}

	if (ImGui::Button("Bake", ImVec2(141.0f * ((float)curScreenWidth / 640.0f), 19.0f * ((float)curScreenHeight / 480.0f))))
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
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetMainViewport()->Size.y - (5.0f * ((float)curScreenHeight / 480.0f) + 25.0f)));
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

	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->Size.x - (141.0f * ((float)curScreenWidth / 640.0f) + 14.0f), ImGui::GetMainViewport()->Size.y - (38.0f * ((float)curScreenHeight / 480.0f) + 17.0f)));
	ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
	ImGui::Begin("CompareNewLoadBtn", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	if (ImGui::Button("Compare", ImVec2(141.0f * ((float)curScreenWidth / 640.0f), 19.0f * ((float)curScreenHeight / 480.0f))))
	{
	}

	if (ImGui::Button("New Load", ImVec2(141.0f * ((float)curScreenWidth / 640.0f), 19.0f * ((float)curScreenHeight / 480.0f))))
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

void LightmapBaker::Renderer::ToolState::RenderCompareModel()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, Renderer::framebufferWidth, Renderer::framebufferHeight, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF);
	glDepthMask(GL_FALSE);
	glClear(GL_STENCIL_BUFFER_BIT);

	glColor3f(0, 0.3f, 0.3f);
	glBegin(GL_QUADS);
	glVertex2f(compareXPosition, 0);
	glVertex2f(Renderer::framebufferWidth, 0);
	glVertex2f(Renderer::framebufferWidth, Renderer::framebufferHeight);
	glVertex2f(compareXPosition, Renderer::framebufferHeight);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspectRatio = Renderer::screenHeight / (float)Renderer::screenWidth;
	Camera camera = Renderer::camera;
	gluPerspective(80, aspectRatio, 0.01f, camera.distance * 2 + 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float camX = (camera.distance * -sinf(camera.angle.x * (M_PI / 180)) * cosf((camera.angle.y) * (M_PI / 180))) + camera.position.x;
	float camY = (camera.distance * -sinf((camera.angle.y) * (M_PI / 180))) + camera.position.y;
	float camZ = (-camera.distance * cosf((camera.angle.x) * (M_PI / 180)) * cosf((camera.angle.y) * (M_PI / 180))) + camera.position.z;
	gluLookAt(camX, camY, camZ,
		camera.position.x, camera.position.y, camera.position.z,
		0.0, 1.0, 0.0);

	glStencilFunc(GL_EQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDepthMask(GL_TRUE);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	switch (compareIndex)
	{
	case 0: // basic
	{
		const auto& vec = Light::RadiosityManager::GetInstance().models;
		for (int i = 0; i < vec.size(); ++i)
		{
			vec[i]->Render();
		}
	}
		break;
	case 1: // radiosity
	{
		const auto& vec = Light::RadiosityManager::GetInstance().elements;
		for (int i = 0; i < vec.size(); ++i)
		{
			vec[i]->mesh->Render();
		}
	}
		break;
	default:
		break;
	}

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void LightmapBaker::Renderer::ToolState::ProgressUI(const std::string& text)
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - (36.0f * ((float)curScreenHeight / 480.0f) + 19.0f)));
	ImGui::Begin("Progress", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text(text.c_str());
	ImGui::End();
}

void LightmapBaker::Renderer::ToolState::CompareUI()
{
	ImGui::SetNextWindowPos(ImVec2((ImGui::GetMainViewport()->Size.x / 2.0f) - 145, ImGui::GetMainViewport()->Size.y - (19.0f * ((float)curScreenHeight / 480.0f) + 36.0f)));
	ImGui::Begin("Compare Helper", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
	ImGui::Text("Move splitter by left / right arrow");
	ImGui::End();

	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)))
	{
		compareXPosition -= ImGui::GetIO().DeltaTime * 1000.0f;
		if (compareXPosition < 0.0f) compareXPosition = 0.0f;
	}
	if (ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_RightArrow)))
	{
		compareXPosition += ImGui::GetIO().DeltaTime * 1000.0f;
		if (ImGui::GetMainViewport()->Size.x < compareXPosition) compareXPosition = ImGui::GetMainViewport()->Size.x;
	}
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
		compareXPosition = ImGui::GetMainViewport()->Size.x;
	}
	break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION:
	{
		// Calc Radiosity
		if (!Light::RadiosityManager::GetInstance().Update())
		{
			compareXPosition = (ImGui::GetMainViewport()->Size.x / 2.0f);
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

		glGenTextures(1, &Light::RadiosityManager::GetInstance().texture);
		glBindTexture(GL_TEXTURE_2D, Light::RadiosityManager::GetInstance().texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load("lightmap.png", &width, &height, &nrChannels, 0);
		if (data)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else
			std::cout << "texture load failed" << std::endl;

		stbi_image_free(data);

		compareXPosition = (ImGui::GetMainViewport()->Size.x / 2.0f);

		auto renderer = Light::RadiosityManager::GetInstance().GetRenderer();
		renderer->renderMeshList.clear();

		std::vector<std::shared_ptr<Mesh>> copiedMeshList = lightMap->GetAtlasUVMesh();
		for (auto& mesh : copiedMeshList)
		{
			renderer->AddRenderMesh(mesh);
		}

		lightMap->Destroy();

		UpdateCurrentState(ToolStateEnum::AFTER_LIGHTMAP_BAKE);
	}
	break;
	case LightmapBaker::Renderer::ToolStateEnum::AFTER_LIGHTMAP_BAKE:
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Light::RadiosityManager::GetInstance().texture);
		break;
	default:
		break;
	}
}

void LightmapBaker::Renderer::ToolState::RenderCurrentUI()
{
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
		RenderCompareModel();
		RenderBeforeLightmapBakeUI();
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_LIGHTMAP_BAKE:
		RenderProgressLightmapBakeUI();
		break;
	case LightmapBaker::Renderer::ToolStateEnum::AFTER_LIGHTMAP_BAKE:
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		RenderCompareModel();
		RenderAfterLightmapBakeUI();
		break;
	default:
		break;
	}
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
}

void LightmapBaker::Renderer::ToolState::SetFrame(double frame)
{
	frames.push_back(frame);
}


float LightmapBaker::Renderer::ToolState::GetCompareXPosition()
{
	return compareXPosition;
}

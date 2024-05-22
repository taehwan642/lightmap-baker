#include "ToolState.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <vector>
#include <thread>
#include "../Light/Light.hpp"
#include "../Data/DataManager.hpp"
#include "../Renderer/Renderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

LightmapBaker::Renderer::ToolState::ToolState()
{
	uis.push_back(std::make_shared<BeforeRadiosityCalculationUI>());
	uis.push_back(std::make_shared<ProgressRadiosityCalculationUI>());
	uis.push_back(std::make_shared<BeforeLightmapBakeUI>());
	uis.push_back(std::make_shared<ProgressLightmapBakeUI>());
	uis.push_back(std::make_shared<AfterLightmapBakeUI>());

	uis[(int)ToolStateEnum::BEFORE_RADIOSITY_CALCULATION]->callbacks.emplace("CalculateRadiosityButton", [&] {
		UpdateCurrentState(ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION);
		auto castedPointer = std::dynamic_pointer_cast<ProgressRadiosityCalculationUI>(renderingUI);
		castedPointer->resolutionX = Light::RadiosityManager::GetInstance().hemiCubeRenderTarget->resolution.x;
		castedPointer->resolutionY = Light::RadiosityManager::GetInstance().hemiCubeRenderTarget->resolution.y;
		castedPointer->renderTexture = Light::RadiosityManager::GetInstance().hemiCubeRenderTarget->renderTexture;
		});
	uis[(int)ToolStateEnum::BEFORE_LIGHTMAP_BAKE]->callbacks.emplace("BakeLightmapButton", [&] { UpdateCurrentState(ToolStateEnum::PROGRESS_LIGHTMAP_BAKE); });
	uis[(int)ToolStateEnum::AFTER_LIGHTMAP_BAKE]->callbacks.emplace("CompareButton", [&] {
		auto castedPointer = std::dynamic_pointer_cast<AfterLightmapBakeUI>(renderingUI);
		Renderer::GetInstance().SetSplitterRenderIndex(SplitterType::RIGHT, castedPointer->compareIndex);
		});
	uis[(int)ToolStateEnum::AFTER_LIGHTMAP_BAKE]->callbacks.emplace("NewLoadButton", [&] {
		Light::RadiosityManager::GetInstance().Destroy();
		std::shared_ptr<Data::DataManager> dataManager = std::make_shared<Data::DataManager>();
		Light::RadiosityManager::GetInstance().Initialize(dataManager->Load());
		UpdateCurrentState(ToolStateEnum::BEFORE_RADIOSITY_CALCULATION);
		});
	frameUI = std::make_shared<FrameUI>();
	frameUI->InitializeUI();
	loggerUI = std::make_shared<LoggerUI>();
	loggerUI->InitializeUI();
}

void LightmapBaker::Renderer::ToolState::UpdateCurrentState(const ToolStateEnum& state)
{
	currentState = state;
	renderingUI = uis[(int)state];
	renderingUI->InitializeUI();
}

void LightmapBaker::Renderer::ToolState::Update()
{
	switch (currentState)
	{
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_RADIOSITY_CALCULATION:
	{
	}
	break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION:
	{
		// Calc Radiosity
		if (!Light::RadiosityManager::GetInstance().Update())
		{
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
		if (threadState == ThreadState::RUNNING)
			break;

		if (threadLightmap == nullptr)
			threadLightmap = std::make_shared<Light::Lightmap>();

		if (threadState == ThreadState::DONE)
		{
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
				LoggerUI::AddLog("texture load failed");

			stbi_image_free(data);
			glBindTexture(GL_TEXTURE_2D, 0);

			Renderer& renderer = Renderer::GetInstance();
			renderer.ClearRenderMesh(SplitterType::LEFT);

			std::vector<std::shared_ptr<Mesh>> copiedMeshList = threadLightmap->GetAtlasUVMesh();
			for (auto& mesh : copiedMeshList)
			{
				mesh->texture = Light::RadiosityManager::GetInstance().texture;
				renderer.AddRenderMesh(SplitterType::LEFT, mesh);
			}

			threadLightmap = nullptr;

			UpdateCurrentState(ToolStateEnum::AFTER_LIGHTMAP_BAKE);
			break;
		}

		std::thread bakingThread([&]
			{
				std::vector<std::shared_ptr<Mesh>> meshList;
				for (int i = 0; i < Light::RadiosityManager::GetInstance().elements.size(); ++i)
				{
					meshList.push_back(Light::RadiosityManager::GetInstance().elements[i]->mesh);
				}

				threadLightmap->Bake(meshList);
				std::shared_ptr<Data::DataManager> dataManager = std::make_shared<Data::DataManager>();
				if (!dataManager->Save("lightmap.png", threadLightmap))
					LoggerUI::AddLog("Bake Error");

				threadState = ThreadState::DONE;
			});

		bakingThread.detach();
		threadState = ThreadState::RUNNING;
	}
	break;
	case LightmapBaker::Renderer::ToolStateEnum::AFTER_LIGHTMAP_BAKE:
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, Light::RadiosityManager::GetInstance().texture);
		break;
	default:
		break;
	}
	frameUI->AddFPS(1.0 / Renderer::GetInstance().deltaTime);
}

void LightmapBaker::Renderer::ToolState::RenderCurrentUI()
{
	switch (currentState)
	{
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_RADIOSITY_CALCULATION:
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION:
		break;
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_LIGHTMAP_BAKE:
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_LIGHTMAP_BAKE:
		break;
	case LightmapBaker::Renderer::ToolStateEnum::AFTER_LIGHTMAP_BAKE:
		break;
	default:
		break;
	}
	renderingUI->RenderUI();
	frameUI->RenderUI();
	loggerUI->RenderUI();
}

float LightmapBaker::Renderer::ToolState::GetSplitXPosition()
{
	auto castedPointer = std::dynamic_pointer_cast<CompareUI>(renderingUI);
	return castedPointer == nullptr ? ImGui::GetMainViewport()->Size.x : castedPointer->splitPositionX;
}

bool LightmapBaker::Renderer::ToolState::GetIsDrawMeshLine()
{
	auto castedPointer = std::dynamic_pointer_cast<CompareUI>(renderingUI);
	return castedPointer == nullptr ? false : castedPointer->isDrawMeshLine;
}

GLenum LightmapBaker::Renderer::ToolState::GetMeshDrawMode()
{
	return GetIsDrawMeshLine() == true ? (GLenum)GL_LINES : (GLenum)GL_TRIANGLES;
}

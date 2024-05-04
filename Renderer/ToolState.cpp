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

void LightmapBaker::Renderer::ToolState::RenderCompareModel(std::shared_ptr<CompareUI> compareUI)
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
	glVertex2f(compareUI->floatData, 0);
	glVertex2f(Renderer::framebufferWidth, 0);
	glVertex2f(Renderer::framebufferWidth, Renderer::framebufferHeight);
	glVertex2f(compareUI->floatData, Renderer::framebufferHeight);
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

	// TODO: optimize every frame dynamic pointer casting
	auto castedPointer = std::dynamic_pointer_cast<AfterLightmapBakeUI>(renderingUI);
	int compareIndex = 0;
	if (castedPointer == nullptr)
	{
		compareIndex = 0;
	}
	else
	{
		compareIndex = castedPointer->integerData;
	}

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

void LightmapBaker::Renderer::ToolState::Initialize()
{
	uis.push_back(std::make_shared<BeforeRadiosityCalculationUI>());
	uis.push_back(std::make_shared<ProgressRadiosityCalculationUI>());
	uis.push_back(std::make_shared<BeforeLightmapBakeUI>());
	uis.push_back(std::make_shared<ProgressLightmapBakeUI>());
	uis.push_back(std::make_shared<AfterLightmapBakeUI>());

	uis[(int)ToolStateEnum::BEFORE_RADIOSITY_CALCULATION]->callbacks.emplace("CalculateRadiosityButton", [&] { UpdateCurrentState(ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION); });
	uis[(int)ToolStateEnum::BEFORE_LIGHTMAP_BAKE]->callbacks.emplace("BakeLightmapButton", [&] { UpdateCurrentState(ToolStateEnum::PROGRESS_LIGHTMAP_BAKE); } );
	uis[(int)ToolStateEnum::AFTER_LIGHTMAP_BAKE]->callbacks.emplace("NewLoadButton", [&] {
		Light::RadiosityManager::GetInstance().Destroy();
		Light::RadiosityManager::GetInstance().Initialize();
		UpdateCurrentState(ToolStateEnum::BEFORE_RADIOSITY_CALCULATION); 
	});
}

void LightmapBaker::Renderer::ToolState::UpdateCurrentState(const ToolStateEnum& state)
{
	currentState = state;
	renderingUI = uis[(int)state];
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
	renderingUI->RenderUI();
	switch (currentState)
	{
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_RADIOSITY_CALCULATION:
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_RADIOSITY_CALCULATION:
		break;
	case LightmapBaker::Renderer::ToolStateEnum::BEFORE_LIGHTMAP_BAKE:
		RenderCompareModel(std::dynamic_pointer_cast<CompareUI>(renderingUI));
		break;
	case LightmapBaker::Renderer::ToolStateEnum::PROGRESS_LIGHTMAP_BAKE:
		break;
	case LightmapBaker::Renderer::ToolStateEnum::AFTER_LIGHTMAP_BAKE:
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		RenderCompareModel(std::dynamic_pointer_cast<CompareUI>(renderingUI));
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
	auto castedPointer = std::dynamic_pointer_cast<CompareUI>(renderingUI);
	return castedPointer == nullptr ? ImGui::GetMainViewport()->Size.x : castedPointer->floatData;
}

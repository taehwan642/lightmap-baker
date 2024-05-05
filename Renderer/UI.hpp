#pragma once
#include "GLUT/glut.h"
#include <string>
#include <functional>
#include <map>
#include <list>

namespace LightmapBaker
{
namespace Renderer
{
	class UI abstract
	{
	public:
		static int screenWidth;
		static int screenHeight;
		std::map<std::string, std::function<void(void)>> callbacks;
		virtual void RenderUI() = 0;
	};

	class ProgressUI : public UI
	{
	protected:
		std::string text = "";
	public:
		virtual void RenderUI() override;
	};

	class CompareUI : public UI
	{
	public:
		float floatData = 0.0f;
		virtual void RenderUI() override;
	};

	class BeforeRadiosityCalculationUI : public UI
	{
	public:
		virtual void RenderUI() override;
	};
	
	class ProgressRadiosityCalculationUI : public ProgressUI
	{
	public:
		int resolutionX = 0;
		int resolutionY = 0;
		GLuint renderTexture;
		virtual void RenderUI() override;
	};

	class BeforeLightmapBakeUI : public CompareUI
	{
	public:
		virtual void RenderUI() override;
	};

	class ProgressLightmapBakeUI : public ProgressUI
	{
	public:
		virtual void RenderUI() override;
	};

	class AfterLightmapBakeUI : public CompareUI
	{
	public:
		int integerData = 0;
		virtual void RenderUI() override;
	};
}
}
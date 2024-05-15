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
		virtual void InitializeUI() = 0;
		virtual void RenderUI() = 0;
	};

	class ProgressUI : public UI
	{
	protected:
		std::string text = "";
	public:
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

	class CompareUI : public UI
	{
	public:
		float splitPositionX = 0.0f;
		bool isDrawMeshLine = false;
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

	class BeforeRadiosityCalculationUI : public UI
	{
	public:
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};
	
	class ProgressRadiosityCalculationUI : public ProgressUI
	{
	public:
		int resolutionX = 0;
		int resolutionY = 0;
		GLuint renderTexture;
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

	class BeforeLightmapBakeUI : public CompareUI
	{
	public:
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

	class ProgressLightmapBakeUI : public ProgressUI
	{
	public:
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

	class AfterLightmapBakeUI : public CompareUI
	{
	public:
		int compareIndex = 0;
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

	class FrameUI : public UI
	{
	private:
		std::list<double> fpsList;
	public:
		void AddFPS(double fps);
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

	class LoggerUI : public UI
	{
	private:
		bool isEnabled = true;
		static std::list<std::string> logs;
	public:
		static void AddLog(const std::string& log);
		virtual void InitializeUI() override;
		virtual void RenderUI() override;
	};

}
}
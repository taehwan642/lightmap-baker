#pragma once
#include "GLUT/glut.h"

namespace LightmapBaker
{
    namespace Renderer
    {
        enum class SplitterType
        {
            LEFT,
            RIGHT
        };

		enum class ToolStateEnum
		{
			BEFORE_RADIOSITY_CALCULATION,
			PROGRESS_RADIOSITY_CALCULATION,
			BEFORE_LIGHTMAP_BAKE,
			PROGRESS_LIGHTMAP_BAKE,
			AFTER_LIGHTMAP_BAKE
		};

		enum class ThreadState
		{
			IDLE,
			RUNNING,
			DONE
		};
    }
}

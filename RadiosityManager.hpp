#pragma once

#include <iostream>
#include <vector>
#include "GLM/glm.hpp"
#include "GLM/vec3.hpp"
#include "GLM/vec4.hpp"

// these struct is template struct. just for erase errors.
// so it is anywhere don't use, have destination of deleting.
struct Patch {
	glm::vec3 unShotRadiosity;
	glm::vec3 center;
	glm::vec3 normal;
	float area;
};

struct FindResult {
	int shootPatchIndex;
	bool found;
};

namespace LightmapBaker
{
	namespace Renderer
	{
		class RadiosityManager
		{
		private:
			std::vector<Patch> patches;
			std::vector<float> formFactors;

			float totalEnergy;
			float radiosityParameter;

		public: // finish
			std::vector<float> makeTopFactors(int halfResolution);
			std::vector<float> makeSideFactors(int halfResolution);

			int doOneIteration(void);
			FindResult findShootPatch(void);
			void sumFormFactors(std::vector<float>& formFactors, int resolutionX, int resolutionY, std::vector<int>& buffer, std::vector<float>& deltaFactors, int startY);

		public: // processing
			void beginDrawHemiCube(glm::vec4 planeEquation);
			void drawHemiCubeElement(Patch& element, int index);
			void endDrawHemiCube(void);

			void computeFormFactors(int shootPatchIndex);
			void distributeRadiosity(int shootPatchIndex);
		};
	}
}

#pragma once

#include <memory>
#include "GLM/vec3.hpp"
#include "../Renderer/Mesh.hpp"

namespace LightmapBaker
{
namespace Light
{
    class View {
    public:
        glm::vec3 center;
        glm::vec3 lookAt;
        glm::vec3 up;
        float fovX;
        float fovY;
        float viewNear;
        float viewFar;
        int resolutionX;
        int resolutionY;
        void* buffer;
        View(glm::vec3 center, glm::vec3 lookAt, glm::vec3 up, float fovX, float fovY, float viewNear, float viewFar, int resolutionX, int resolutionY, void* buffer) {
            this->center = center;
            this->lookAt = lookAt;
            this->up = up;
            this->fovX = fovX;
            this->fovY = fovY;
            this->viewNear = viewNear;
            this->viewFar = viewFar;
            this->resolutionX = resolutionX;
            this->resolutionY = resolutionY;
            this->buffer = buffer;
        }
    };

    class Radiosity
    {
    public:
        float threshold;
        std::shared_ptr<View> view;
        int hemiCubeResolution;
        float worldSize;
        float intensityScale;
        float addAmbient;
        Radiosity(float threshold, std::shared_ptr<View> view, int hemiCubeResolution, float worldSize, float intensityScale, float addAmbient)
        {
            this->threshold = threshold;
            this->view = view;
            this->hemiCubeResolution = hemiCubeResolution;
            this->worldSize = worldSize;
            this->intensityScale = intensityScale;
            this->addAmbient = addAmbient;
        }
    };

    class Patch
    {
    public:
        glm::vec3 reflectance;
        glm::vec3 emission;
        glm::vec3 center;
        glm::vec3 normal;
        glm::vec3 unShotRadiosity;
        float area;
        Patch(glm::vec3 reflectance, glm::vec3 emission, glm::vec3 center, glm::vec3 normal, glm::vec3 unShotRadiosity, float area)
        {
            this->reflectance = reflectance;
            this->emission = emission;
            this->center = center;
            this->normal = normal;
            this->unShotRadiosity = unShotRadiosity;
            this->area = area;
        }
    };

    class Element
    {
    public:
        std::shared_ptr<Renderer::Mesh> mesh;
        std::vector<GLubyte> indices;
        glm::vec3 normal;
        glm::vec3 radiosity;
        float area;
        std::shared_ptr<Patch> parentPatch;
        glm::vec3 reflectance;
        Element(std::shared_ptr<Renderer::Mesh> mesh, std::vector<GLubyte> indices, glm::vec3 normal, glm::vec3 radiosity, float area, std::shared_ptr<Patch> parentPatch, glm::vec3 reflectance)
        {
            this->mesh = mesh;
            this->indices = indices;
            this->normal = normal;
            this->radiosity = radiosity;
            this->area = area;
            this->parentPatch = parentPatch;
            this->reflectance = reflectance;
        }
    };

    class HemiCube
    {
    public:
        std::shared_ptr<View> view;
        std::vector<float> topFactors;
        std::vector<float> sideFactors;
        HemiCube(std::shared_ptr<View> view, std::vector<float> topFactors, std::vector<float> sideFactors)
        {
            this->view = view;
            this->topFactors = topFactors;
            this->sideFactors = sideFactors;
        }
    };

    class RadiosityManager
    {
    public:
        void Initialize();
        void Update();
        void InitRadiosityParameter();
    };
}
}

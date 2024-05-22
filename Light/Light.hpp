#pragma once

#include <memory>
#include "GLM/vec3.hpp"
#include "../Renderer/RenderTarget.hpp"
#include "../Renderer/Mesh.hpp"
#include "../Renderer/Camera.hpp"
#include "../Utility/Singleton.hpp"
#include "GLUT/glut.h"

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
        std::vector<int> buffer;
        View(glm::vec3 center, glm::vec3 lookAt, glm::vec3 up, float fovX, float fovY, float viewNear, float viewFar, int resolutionX, int resolutionY, std::vector<int> buffer) {
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
        Patch() = default;
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
        std::vector<GLuint> indices;
        glm::vec3 normal;
        glm::vec3 radiosity;
        float area;
        std::shared_ptr<Patch> parentPatch;
        glm::vec3 reflectance;
        Element(std::shared_ptr<Renderer::Mesh> mesh, std::vector<GLuint> indices, glm::vec3 normal, glm::vec3 radiosity, float area, std::shared_ptr<Patch> parentPatch, glm::vec3 reflectance)
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

    class RadiosityManager : public Utility::Singleton<RadiosityManager>
    {
    private:
        RadiosityManager() = default;
        ~RadiosityManager() = default;
        friend class Utility::Singleton<RadiosityManager>;

    public:
        std::vector<std::shared_ptr<Renderer::Mesh>> models;
        std::vector<std::shared_ptr<Patch>> patches;
        std::vector<std::shared_ptr<Element>> elements;
        std::shared_ptr<Radiosity> radiosity;
        std::shared_ptr<HemiCube> hemiCube;
        std::vector<Renderer::Vertex> subDividedVertices;
        std::vector<Renderer::Vertex> subDividedConvertVertices;
        std::vector<float> formFactors;
        float totalEnergy;

        std::shared_ptr<Renderer::RenderTarget> hemiCubeRenderTarget;
        std::shared_ptr<Renderer::Camera> hemiCubeCamera;

        std::vector<UINT8> readBuffer;

        unsigned int texture;
    public:
        void SubDivideMesh(std::shared_ptr<Renderer::Mesh> modelData, int& vertexOffset, int& patchesIndex, int& elementIndex);
        glm::vec3 ConvertUVtoPoint(std::vector<glm::vec3> vertices, float u, float v);
        glm::vec2 ConvertUVtoPoint(std::vector<glm::vec2> vertices, float u, float v);
        std::vector<float> MakeTopFactors(int halfResolution);
        std::vector<float> MakeSideFactors(int halfResolution);

        bool DoOneIteration();
        bool FindShootPatch(int& shootPatchIndex);
        void SumFormFactors(int resolutionX, int resolutionY, std::vector<int>& buffer, std::vector<float>& deltaFactors, int startY);

        void BeginDrawHemiCube();
        void DrawHemiCubeElement(std::shared_ptr<Element> element, int index);
        void EndDrawHemiCube();

        void ComputeFormFactors(int shootPatchIndex);
        void DistributeRadiosity(int shootPatchIndex);
    public:
        void Initialize(std::vector<std::shared_ptr<Renderer::Mesh>> modelsInput);
        bool Update();
        void Destroy();
        float InitRadiosityParameter();
    };
}
}

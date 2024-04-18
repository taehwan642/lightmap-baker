#include "GLEW/glew.h"
#include "Light.hpp"
#include <cmath>
#include "../Data/DataManager.hpp"
#include "GLFW/glfw3.h"
#include "GLM/vec2.hpp"
#include "GLM/geometric.hpp"

void LightmapBaker::Light::RadiosityManager::Initialize()
{
    radiosity = std::make_shared<Radiosity>(
        0.0001,
        std::make_shared<View>(
            glm::vec3(150, 50, 700),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0),
            60, 60,
            1, 550,
            200, 200,
            std::vector<UINT8>()),
        100,
        250,
        50,
        1
    );
    const int hemiCubeResolution = (floor((radiosity->hemiCubeResolution / 2) + 0.5) * 2);
    radiosity->view->buffer.resize(hemiCubeResolution * hemiCubeResolution * 4);
        hemiCube = std::make_shared<HemiCube>(
        std::make_shared<View>(
            glm::vec3(0, 0, 1),
            glm::vec3(0, 0, 0),
            glm::vec3(0, 1, 0),
            90, 90,
            radiosity->worldSize * 0.001,
            radiosity->worldSize,
            hemiCubeResolution,
            hemiCubeResolution,
            std::vector<UINT8>(hemiCubeResolution * hemiCubeResolution * 4)),
        MakeTopFactors(hemiCubeResolution / 2),
        MakeSideFactors(hemiCubeResolution / 2)
    );

    // r g b a -> times 4
    readBuffer.resize(hemiCube->view->resolutionX * hemiCube->view->resolutionY * 4);

    std::shared_ptr<Data::DataManager> dataManager = std::make_shared<Data::DataManager>();
    models = dataManager->Load();
    int patchTotalCount = 0;
    for (int i = 0; i < models.size(); ++i)
    {
        // patch count on mesh = patchLevel * patchLevel
        patchTotalCount += models[i]->patchLevel * models[i]->patchLevel;
    }

    for (int i = 0; i < patchTotalCount; ++i)
    {
        patches.push_back(std::make_shared<Patch>());
    }

    int vertexOffset = 0;
    int patchesIndex = 0;
    int elementIndex = 0;
    for (int i = 0; i < models.size(); ++i)
    {
        SubDivideMesh(models[i], vertexOffset, patchesIndex, elementIndex);
    }

    formFactors.resize(elements.size());
    for (int i = 0; i < elements.size(); ++i)
    {
        formFactors[i] = 0;
    }

    const float totalEnergy = InitRadiosityParameter();

    glm::vec2 renderTargetSize = glm::vec2(hemiCube->view->resolutionX, hemiCube->view->resolutionY);

    glGenFramebuffers(1, &hemiCubeRenderTarget.frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, hemiCubeRenderTarget.frameBuffer);

    glGenTextures(1, &hemiCubeRenderTarget.renderedTexture);

    glBindTexture(GL_TEXTURE_2D, hemiCubeRenderTarget.renderedTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderTargetSize.x, renderTargetSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glGenRenderbuffers(1, &hemiCubeRenderTarget.depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, hemiCubeRenderTarget.depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderTargetSize.x, renderTargetSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hemiCubeRenderTarget.depthrenderbuffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, hemiCubeRenderTarget.renderedTexture, 0);

    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers);

    for (int i = 0; i < elements.size(); ++i)
    {
        std::shared_ptr<Renderer::Mesh> mesh = std::make_shared<Renderer::Mesh>();
        mesh->vertices = subDividedVertices;
        mesh->normal = elements[i]->normal;
        mesh->indices = elements[i]->indices;
        mesh->color = elements[i]->radiosity;

        elements[i]->mesh = mesh;
        renderer->AddRenderMesh(elements[i]->mesh);
    }
}

void LightmapBaker::Light::RadiosityManager::Update()
{
    glBindFramebuffer(GL_FRAMEBUFFER, hemiCubeRenderTarget.frameBuffer);
    DoOneIteration();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < elements.size(); ++i)
    {
        elements[i]->mesh->color = elements[i]->radiosity * radiosity->intensityScale;
    }
}

float LightmapBaker::Light::RadiosityManager::InitRadiosityParameter()
{
    for (int i = 0; i < patches.size(); ++i) {
        patches[i]->unShotRadiosity = patches[i]->emission;
    }

    for (int i = 0; i < elements.size(); ++i) {
        elements[i]->radiosity = elements[i]->parentPatch->emission;
    }

    float totalEnergy = 0.0f;
    for (int i = 0; i < patches.size(); ++i) {
        totalEnergy += patches[i]->emission.r * patches[i]->area;
        totalEnergy += patches[i]->emission.g * patches[i]->area;
        totalEnergy += patches[i]->emission.b * patches[i]->area;
    }
    return totalEnergy;
}

void LightmapBaker::Light::RadiosityManager::SetRenderer(std::shared_ptr<Renderer::Renderer> renderer)
{
    this->renderer = renderer;
}

void LightmapBaker::Light::RadiosityManager::SubDivideMesh(std::shared_ptr<Renderer::Mesh> modelData, int& vertexOffset, int& patchesIndex, int& elementIndex)
{
    std::vector<glm::vec3> quadVertices;
    int nu, nv;
    float du, dv;
    int i, j;
    float u, v;
    int fi, fj;
    int pi, pj;
    int verticesCount = 0;

    // if 0 3 2 0 2 1 and cw, 0 3 2 1 will be quad vertices.
    quadVertices.push_back(modelData->GetVertexByIndex(modelData->indices[0]));
    quadVertices.push_back(modelData->GetVertexByIndex(modelData->indices[1]));
    quadVertices.push_back(modelData->GetVertexByIndex(modelData->indices[2]));
    quadVertices.push_back(modelData->GetVertexByIndex(modelData->indices[5]));

    nu = modelData->patchLevel * modelData->elementLevel + 1;
    nv = modelData->patchLevel * modelData->elementLevel + 1;
    du = 1.0f / (float)(nu - 1);
    dv = 1.0f / (float)(nv - 1);
    for (i = 0, u = 0; i < nu; i++, u += du)
    {
        for (j = 0, v = 0; j < nv; j++, v += dv, verticesCount++)
        {
            subDividedVertices.push_back(ConvertUVtoPoint(quadVertices, u, v));
        }
    }

    nu = modelData->patchLevel * modelData->elementLevel;
    nv = modelData->patchLevel * modelData->elementLevel;
    du = 1.0 / (float)nu;
    dv = 1.0 / (float)nv;
    for (i = 0, u = du / 2.0; i < nu; i++, u += du) {
        for (j = 0, v = dv / 2.0; j < nv; j++, v += dv, elementIndex++) {
            fi = i / nu;
            fj = j / nv;
            pi = floor(fi * (modelData->patchLevel));
            pj = floor(fj * (modelData->patchLevel));

            elements.push_back(std::make_shared<Element>(
                nullptr,
                std::vector<GLubyte>{
                    (GLubyte)((i* (nv + 1) + j) + vertexOffset),
                    (GLubyte)(((i + 1)* (nv + 1) + (j + 1)) + vertexOffset),
                    (GLubyte)(((i + 1)* (nv + 1) + j) + vertexOffset),
                    (GLubyte)((i* (nv + 1) + j) + vertexOffset),
                    (GLubyte)((i* (nv + 1) + (j + 1)) + vertexOffset),
                    (GLubyte)(((i + 1)* (nv + 1) + (j + 1)) + vertexOffset)
                },
                modelData->normal,
                glm::vec3(0, 0, 0),
                modelData->area / (nu * nv),
                patches[patchesIndex + pi * modelData->patchLevel + pj],
                modelData->reflectance
            ));
        }
    }

    /* Calculate patches */
    nu = modelData->patchLevel;
    nv = modelData->patchLevel;
    du = 1.0f / (float)nu;
    dv = 1.0f / (float)nv;
    for (i = 0, u = du / 2.0; i < nu; i++, u += du)
    {
        for (j = 0, v = dv / 2.0; j < nv; j++, v += dv, patchesIndex++)
        {
            patches[patchesIndex]->center = ConvertUVtoPoint(quadVertices, u, v);
            patches[patchesIndex]->normal = modelData->normal;
            patches[patchesIndex]->reflectance = modelData->reflectance;
            patches[patchesIndex]->emission = modelData->emission;
            patches[patchesIndex]->area = modelData->area / (nu * nv);
        }
    }
    vertexOffset += verticesCount;

    return;
}

glm::vec3 LightmapBaker::Light::RadiosityManager::ConvertUVtoPoint(std::vector<glm::vec3> vertices, float u, float v)
{
    glm::vec3 point = glm::vec3(
        vertices[0].x * (1 - u) * (1 - v) + vertices[1].x * (1 - u) * v + vertices[2].x * u * v + vertices[3].x * u * (1 - v),
        vertices[0].y * (1 - u) * (1 - v) + vertices[1].y * (1 - u) * v + vertices[2].y * u * v + vertices[3].y * u * (1 - v),
        vertices[0].z * (1 - u) * (1 - v) + vertices[1].z * (1 - u) * v + vertices[2].z * u * v + vertices[3].z * u * (1 - v)
        );
    return point;
}

std::vector<float> LightmapBaker::Light::RadiosityManager::MakeTopFactors(int halfResolution)
{
    const float PI = 3.1415926f;
    int j, k;
    float xSq, ySq, xy1Sq;
    int n = halfResolution;
    std::vector<float> wp(n * n);
    int index = 0;

    for (j = 0; j < halfResolution; j++)
    {
        float dj = j;
        ySq = (n - (dj + 0.5f)) / n;
        ySq *= ySq;
        for (k = 0; k < halfResolution; k++)
        {
            float dk = k;
            xSq = (n - (dk + 0.5f)) / n;
            xSq *= xSq;
            xy1Sq = xSq + ySq + 1.0f;
            xy1Sq *= xy1Sq;
            wp[index++] = 1.0f / (xy1Sq * PI * n * n);
        }
    }

    return wp;
}

std::vector<float> LightmapBaker::Light::RadiosityManager::MakeSideFactors(int halfResolution)
{
    const float PI = 3.1415926f;
    int j, k;
    float x, xSq, y, ySq, xy1, xy1Sq;
    int n = halfResolution;
    std::vector<float> wp(n * n);
    int index = 0;

    for (j = 0; j < halfResolution; j++)
    {
        float dj = j;
        y = (n - (dj + 0.5f)) / n;
        ySq = y * y;
        for (k = 0; k < halfResolution; k++)
        {
            float dk = k;
            x = (n - (dk + 0.5f)) / n;
            xSq = x * x;
            xy1 = xSq + ySq + 1.0f;
            xy1Sq = xy1 * xy1;
            wp[index++] = y / (xy1Sq * PI * n * n);
        }
    }

    return wp;
}

int LightmapBaker::Light::RadiosityManager::DoOneIteration()
{
    int shootPatchIndex = 0;
    if (FindShootPatch(shootPatchIndex))
    {
        ComputeFormFactors(shootPatchIndex);
        DistributeRadiosity(shootPatchIndex);
        return 0;
    }

    return 1;
}

bool LightmapBaker::Light::RadiosityManager::FindShootPatch(int& shootPatchIndex)
{
    float energySum, error, maxEnergySum = 0;

    for (size_t i = 0; i < patches.size(); i++)
    {
        energySum = 0;
        energySum += patches[i]->unShotRadiosity.r * patches[i]->area;
        energySum += patches[i]->unShotRadiosity.g * patches[i]->area;
        energySum += patches[i]->unShotRadiosity.b * patches[i]->area;

        if (energySum > maxEnergySum)
        {
            shootPatchIndex = i;
            maxEnergySum = energySum;
        }
    }

    error = maxEnergySum / totalEnergy;
    return !(error < radiosity->threshold);
}

void LightmapBaker::Light::RadiosityManager::SumFormFactors(int resolutionX, int resolutionY, std::vector<UINT8>& buffer, std::vector<float>& deltaFactors, int startY)
{
    int i, j;
    int ii, jj;
    int halfResolution = resolutionX / 2;
    int current_backItem = 16777215;
    for (i = startY; i < resolutionY; i++)
    {
        ii = (i < halfResolution ? i : (halfResolution - 1 - (i % halfResolution))) * halfResolution;
        for (j = 0; j < resolutionX; j++)
        {
            if (buffer[i * resolutionX + j] != current_backItem)
            {
                jj = (j < halfResolution ? j : (halfResolution - 1 - (j % halfResolution)));
                formFactors[buffer[i * resolutionX + j]] += deltaFactors[ii + jj];
            }
        }
    }
}

void LightmapBaker::Light::RadiosityManager::BeginDrawHemiCube(glm::vec4 planeEquation)
{
    /* clear the frame buffer with color */
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gluLookAt(
        hemiCube->view->lookAt.x, hemiCube->view->lookAt.y, hemiCube->view->lookAt.z,
        hemiCube->view->center.x, hemiCube->view->center.y, hemiCube->view->center.z,
        hemiCube->view->up.x, hemiCube->view->up.y, hemiCube->view->up.z);
}

void LightmapBaker::Light::RadiosityManager::DrawHemiCubeElement(std::shared_ptr<Element> element, int index)
{
    element->mesh->color = glm::vec3((index / 65536) / 255, ((index % 65536) / 256) / 255, (index % 256) / 255);
}

void LightmapBaker::Light::RadiosityManager::EndDrawHemiCube()
{
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, readBuffer.data());
    std::vector<UINT8> newBuffer;
    int index = 0;
    for (int i = 0; i < readBuffer.size(); ++i)
    {
        if (index == 3)
        {
            index = 0;
            continue;
        }
        newBuffer.push_back(readBuffer[i]);
        ++index;
    }

    for (int j = 0; j < hemiCube->view->resolutionY; ++j)
    {
        for (int i = 0; i < hemiCube->view->resolutionX; ++i)
        {
            // r g b a
            hemiCube->view->buffer[i * hemiCube->view->resolutionY + j] =
                (newBuffer[i * 3 * hemiCube->view->resolutionY + j * 3] * 65536) +
                (newBuffer[i * 3 * hemiCube->view->resolutionY + j * 3 + 1] * 256) +
                (newBuffer[i * 3 * hemiCube->view->resolutionY + j * 3 + 2]);
        }
    }
}

void LightmapBaker::Light::RadiosityManager::ComputeFormFactors(int shootPatchIndex)
{
    std::shared_ptr<Patch> shootPatch = patches[shootPatchIndex];

    std::vector<glm::vec3> up;
    std::vector<glm::vec3> lookAt;
    glm::vec3 center = glm::vec3(shootPatch->center.x, shootPatch->center.y, shootPatch->center.z);
    glm::vec3 normal = glm::vec3(shootPatch->normal.x, shootPatch->normal.y, shootPatch->normal.z);
    glm::vec3 tangentU;
    glm::vec3 tangentV;
    glm::vec3 vec;
    float norm = 0;


    glm::vec4 planeEquation;
    planeEquation.x = shootPatch->normal.x;
    planeEquation.y = shootPatch->normal.y;
    planeEquation.z = shootPatch->normal.z;
    planeEquation.w = -(shootPatch->normal.x * shootPatch->center.x + 
        shootPatch->normal.y * shootPatch->center.y + 
        shootPatch->normal.z * shootPatch->center.z);

    do 
    {
        vec.x = rand() % 10;
        vec.y = rand() % 10;
        vec.z = rand() % 10;
        tangentU = glm::cross(normal, vec);
        norm = tangentU.length();
        tangentU /= tangentU.length();
    } while (norm == 0);
    tangentV = glm::cross(normal, tangentU);

    lookAt.push_back(center + normal);
    up.push_back(tangentU);
    lookAt.push_back(center + tangentU);
    up.push_back(normal);
    lookAt.push_back(center + tangentV);
    up.push_back(normal);
    lookAt.push_back(center - tangentU);
    up.push_back(normal);
    lookAt.push_back(center - tangentV);
    up.push_back(normal);

    normal *= radiosity->worldSize * 0.00001;
    hemiCube->view->center = center + normal;

    // /* clear the formfactors */
    for (int i = 0; i < formFactors.size(); ++i)
    {
        formFactors[i] = 0;
    }

    for (int face = 0; face < 5; ++face)
    {

        hemiCube->view->lookAt = lookAt[face];
        hemiCube->view->up = up[face];

        BeginDrawHemiCube(planeEquation);
        for (int i = 0; i < elements.size(); ++i)
        {
            DrawHemiCubeElement(elements[i], i);
        }

        // render by hemicubecamera


        EndDrawHemiCube();

        if (face == 0)
        {
            SumFormFactors(hemiCube->view->resolutionX, hemiCube->view->resolutionY, hemiCube->view->buffer, hemiCube->topFactors, 0);
        }
        else
        {
            SumFormFactors(hemiCube->view->resolutionX, hemiCube->view->resolutionY, hemiCube->view->buffer, hemiCube->sideFactors, hemiCube->view->resolutionY / 2);
        }
    }

    for (int i = 0; i < elements.size(); ++i)
    {
        formFactors[i] *= shootPatch->area / elements[i]->area;

        if (formFactors[i] > 1.0f) {
            formFactors[i] = 1.0f;
        }
    }
}

void LightmapBaker::Light::RadiosityManager::DistributeRadiosity(int shootPatchIndex)
{
    glm::vec3 deltaRad = glm::vec3(0, 0, 0);
    float w;

    std::shared_ptr<Patch> shootPatch = patches[shootPatchIndex];

    for (int i = 0; i < elements.size(); ++i) {
        if (formFactors[i] != 0) {
            deltaRad.r = shootPatch->unShotRadiosity.r * formFactors[i] * elements[i]->parentPatch->reflectance.r;
            deltaRad.g = shootPatch->unShotRadiosity.g * formFactors[i] * elements[i]->parentPatch->reflectance.g;
            deltaRad.b = shootPatch->unShotRadiosity.b * formFactors[i] * elements[i]->parentPatch->reflectance.b;
            /* incremental element's radiosity and patch's unshot radiosity */
            w = elements[i]->area / elements[i]->parentPatch->area;

            elements[i]->radiosity += deltaRad;
            elements[i]->parentPatch->unShotRadiosity += deltaRad * w;
        }
    }
    /* reset shooting patch's unshot radiosity */
    shootPatch->unShotRadiosity = glm::vec3(0, 0, 0);
}

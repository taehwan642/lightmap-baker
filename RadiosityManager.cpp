#include "RadiosityManager.hpp"

std::vector<float> LightmapBaker::Renderer::RadiosityManager::makeTopFactors(int halfResolution)
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

std::vector<float> LightmapBaker::Renderer::RadiosityManager::makeSideFactors(int halfResolution)
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

int LightmapBaker::Renderer::RadiosityManager::doOneIteration(void)
{
    auto findResult = findShootPatch();
    if (findResult.found)
    {
        std::cout << findResult.shootPatchIndex << std::endl;
        computeFormFactors(findResult.shootPatchIndex);
        distributeRadiosity(findResult.shootPatchIndex);
        return 0;
    }

    std::cout << "Radiosity done" << std::endl;
    return 1;
}

FindResult LightmapBaker::Renderer::RadiosityManager::findShootPatch(void)
{
    int shootPatchIndex = 0;
    float energySum, error, maxEnergySum = 0;

    for (size_t i = 0; i < patches.size(); i++)
    {
        energySum = 0;
        energySum += patches[i].unShotRadiosity.r * patches[i].area;
        energySum += patches[i].unShotRadiosity.g * patches[i].area;
        energySum += patches[i].unShotRadiosity.b * patches[i].area;

        if (energySum > maxEnergySum)
        {
            shootPatchIndex = i;
            maxEnergySum = energySum;
        }
    }

    error = maxEnergySum / totalEnergy;
    return { shootPatchIndex, !(error < radiosityParameter) };
}

void LightmapBaker::Renderer::RadiosityManager::sumFormFactors(std::vector<float>& formFactors, int resolutionX, int resolutionY, std::vector<int>& buffer, std::vector<float>& deltaFactors, int startY)
{
    int i, j;
    int ii, jj;
    int halfResolution = resolutionX / 2;
    int current_backItem = 16777215;
    for (i = startY; i < resolutionY; i++) {
        ii = (i < halfResolution ? i : (halfResolution - 1 - (i % halfResolution))) * halfResolution;
        for (j = 0; j < resolutionX; j++) {
            if (buffer[i * resolutionX + j] != current_backItem)
            {
                jj = (j < halfResolution ? j : (halfResolution - 1 - (j % halfResolution)));
                formFactors[buffer[i * resolutionX + j]] += deltaFactors[ii + jj];
            }
        }
    }
}

void LightmapBaker::Renderer::RadiosityManager::beginDrawHemiCube(glm::vec4 planeEquation)
{
    return;
}

void LightmapBaker::Renderer::RadiosityManager::drawHemiCubeElement(Patch& element, int index)
{
    //element.mesh.material.color = new glm::vec3(glm::trunc(index / 65536) / 255, glm::trunc((index % 65536) / 256) / 255, glm::trunc(index % 256) / 255);
    return;
}

void LightmapBaker::Renderer::RadiosityManager::endDrawHemiCube(void)
{
    return;
}

void LightmapBaker::Renderer::RadiosityManager::computeFormFactors(int shootPatchIndex)
{
    return;
}

void LightmapBaker::Renderer::RadiosityManager::distributeRadiosity(int shootPatchIndex)
{
    return;
}

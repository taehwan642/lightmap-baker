#include "DataManager.hpp"
#include <iostream>
#include <fstream>
#include <stack>
#include "GLFW/glfw3.h"
#include "GLUT/glut.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include "stb/stb_image_write.h"

void LightmapBaker::Data::DataManager::Save(std::string path, int width, int height, const void* data)
{
    stbi_write_png(path.c_str(), width, height, 3, data, 0);
    std::cout << "Save " << " " << path << std::endl;
}

std::vector<std::shared_ptr<LightmapBaker::Renderer::Mesh>> LightmapBaker::Data::DataManager::Load(const std::string& path)
{
    std::cout << "Load " << " " << path << std::endl;

    std::ifstream fout(path, std::ios::in | std::ios::binary);
    std::vector<std::shared_ptr<LightmapBaker::Renderer::Mesh>> resultList;
    std::shared_ptr<LightmapBaker::Renderer::Mesh> mesh = nullptr;
    if (fout.is_open())
    {
        int cnt = 0;
        while (!fout.eof())
        {
            std::string temp;
            std::getline(fout, temp);

            int pos = 0;
            char target = ':';
            for (; pos < temp.size(); pos++)
                if (temp[pos] == target)
                    break;

            switch (cnt)
            {
            case 0: // indices
            {
                mesh = std::make_shared<LightmapBaker::Renderer::Mesh>();

                pos += 2;

                int start = pos;
                for (int i = pos; i < temp.size(); i++)
                {
                    if (temp[i] == ',' || temp[i] == ']')
                    {
                        int num = std::stoi(temp.substr(start, i - start));
                        start = i + 1;

                        mesh->indices.push_back(num);
                    }
                }
            }
            break;
            case 1: // normal
            {
                pos += 2;
                int start = pos;

                int idx = 0;
                for (int i = pos; i < temp.size() - 1; i++)
                {
                    if (temp[i] == ',' || temp[i] == ')')
                    {
                        float v = std::stof(temp.substr(start, i - start));
                        start = i + 1;

                        mesh->normal[idx++] = v;
                    }
                }
            }
            break;
            case 2: // patchlevel
            {
                pos++;
                mesh->patchLevel = std::stoi(temp.substr(pos, temp.size() - 1));
            }
            break;
            case 3: // elementlevel
            {
                pos++;
                mesh->elementLevel = std::stoi(temp.substr(pos, temp.size() - 1));
            }
            break;
            case 4: // area
            {
                pos++;
                mesh->area = std::stof(temp.substr(pos, temp.size() - 1));
            }
            break;
            case 5: // reflectance
            {
                pos += 2;
                int start = pos;

                int idx = 0;
                for (int i = pos; i < temp.size() - 1; i++)
                {
                    if (temp[i] == ',' || temp[i] == ')')
                    {
                        float v = std::stof(temp.substr(start, i - start));
                        start = i + 1;

                        mesh->reflectance[idx++] = v;
                    }
                }
            }
            break;
            case 6: // emmision
            {
                pos += 2;
                int start = pos;

                int idx = 0;
                for (int i = pos; i < temp.size() - 1; i++)
                {
                    if (temp[i] == ',' || temp[i] == ')')
                    {
                        float v = std::stof(temp.substr(start, i - start));
                        start = i + 1;

                        mesh->emission[idx++] = v;
                    }
                }

                resultList.push_back(mesh);
                cnt = 0;

                continue;
            }
            break;
            }
            cnt++;
        }
    }
    else
    {
        std::cout << "FILE LOAD FAIL: " << path << std::endl;
    }

    return resultList;
}

std::vector<std::shared_ptr<LightmapBaker::Renderer::Mesh>> LightmapBaker::Data::DataManager::Load()
{
    using namespace LightmapBaker;
    std::vector<std::shared_ptr<Renderer::Mesh>> meshes;
    const glm::vec3 red = glm::vec3(0.80, 0.10, 0.075);
    const glm::vec3 yellow = glm::vec3(0.9, 0.8, 0.1);
    const glm::vec3 blue = glm::vec3(0.075, 0.10, 0.35);
    const glm::vec3 white = glm::vec3(1.0, 1.0, 1.0);
    const glm::vec3 lightGrey = glm::vec3(0.9, 0.9, 0.9);
    const glm::vec3 black = glm::vec3(0.0, 0.0, 0.0);
    const glm::vec3 lightGreen = glm::vec3(0.63, 0.85, 0.58);
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{4, 5, 6, 4, 6, 7}, glm::vec3(0, -1, 0), 2, 8, (216 * 215), lightGrey, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{0, 3, 2, 0, 2, 1}, glm::vec3(0, 1, 0), 3, 8, (216 * 215), lightGrey, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{0, 4, 7, 0, 7, 3}, glm::vec3(1, 0, 0), 2, 8, (221 * 215), red, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{0, 1, 5, 0, 5, 4}, glm::vec3(0, 0, 1), 2, 8, (221 * 216), lightGrey, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{2, 6, 5, 2, 5, 1}, glm::vec3(-1, 0, 0), 2, 8, (221 * 215), blue, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{2, 3, 7, 2, 7, 6}, glm::vec3(0, 0, -1), 2, 8, (221 * 216), lightGrey, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{8, 9, 10, 8, 10, 11}, glm::vec3(0, -1, 0), 2, 1, (40 * 45), black, white));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{16, 19, 18, 16, 18, 17}, glm::vec3(0, 1, 0), 1, 5, (65 * 65), yellow, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{12, 13, 14, 12, 14, 15}, glm::vec3(0, -1, 0), 1, 1, (65 * 65), yellow, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{12, 15, 19, 12, 19, 16}, glm::vec3(-0.866, 0, -0.5), 1, 5, (65 * 65), yellow, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{12, 16, 17, 12, 17, 13}, glm::vec3(0.5, 0, -0.866), 1, 5, (65 * 65), yellow, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{14, 13, 17, 14, 17, 18}, glm::vec3(0.866, 0, 0.5), 1, 5, (65 * 65), yellow, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{14, 18, 19, 14, 19, 15}, glm::vec3(-0.5, 0, 0.866), 1, 5, (65 * 65), yellow, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{24, 27, 26, 24, 26, 25}, glm::vec3(0, 1, 0), 1, 5, (65 * 65), lightGreen, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{20, 21, 22, 20, 22, 23}, glm::vec3(0, -1, 0), 1, 1, (65 * 65), lightGreen, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{20, 23, 27, 20, 27, 24}, glm::vec3(-0.866, 0, -0.5), 1, 6, (65 * 130), lightGreen, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{20, 24, 25, 20, 25, 21}, glm::vec3(0.5, 0, -0.866), 1, 6, (65 * 130), lightGreen, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{22, 21, 25, 22, 25, 26}, glm::vec3(0.866, 0, 0.5), 1, 6, (65 * 130), lightGreen, black));
    meshes.push_back(std::make_shared<Renderer::Mesh>(GetVertices(), std::vector<GLuint>{22, 26, 27, 22, 27, 23}, glm::vec3(-0.5, 0, 0.866), 1, 6, (65 * 130), lightGreen, black));
    return meshes;
}

std::vector<glm::vec3> LightmapBaker::Data::DataManager::GetVertices()
{
    std::vector<glm::vec3> vertices;
    vertices.push_back(glm::vec3(0, 0, 0));
    vertices.push_back(glm::vec3(216, 0, 0));
    vertices.push_back(glm::vec3(216, 0, 215));
    vertices.push_back(glm::vec3(0, 0, 215));
    vertices.push_back(glm::vec3(0, 221, 0));
    vertices.push_back(glm::vec3(216, 221, 0));
    vertices.push_back(glm::vec3(216, 221, 215));
    vertices.push_back(glm::vec3(0, 221, 215));
    vertices.push_back(glm::vec3(85.5, 220, 90));
    vertices.push_back(glm::vec3(130.5, 220, 90));
    vertices.push_back(glm::vec3(130.5, 220, 130));
    vertices.push_back(glm::vec3(85.5, 220, 130));
    vertices.push_back(glm::vec3(53.104, 0, 64.104));
    vertices.push_back(glm::vec3(109.36, 0, 96.604));
    vertices.push_back(glm::vec3(76.896, 0, 152.896));
    vertices.push_back(glm::vec3(20.604, 0, 120.396));
    vertices.push_back(glm::vec3(53.104, 65, 64.104));
    vertices.push_back(glm::vec3(109.36, 65, 96.604));
    vertices.push_back(glm::vec3(76.896, 65, 152.896));
    vertices.push_back(glm::vec3(20.604, 65, 120.396));
    vertices.push_back(glm::vec3(134.104, 0, 67.104));
    vertices.push_back(glm::vec3(190.396, 0, 99.604));
    vertices.push_back(glm::vec3(157.896, 0, 155.896));
    vertices.push_back(glm::vec3(101.604, 0, 123.396));
    vertices.push_back(glm::vec3(134.104, 130, 67.104));
    vertices.push_back(glm::vec3(190.396, 130, 99.604));
    vertices.push_back(glm::vec3(157.896, 130, 155.896));
    vertices.push_back(glm::vec3(101.604, 130, 123.396));
    return vertices;
}

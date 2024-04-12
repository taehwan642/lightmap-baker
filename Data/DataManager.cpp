#include "DataManager.hpp"
#include <iostream>
#include <fstream>
#include <stack>

void LightmapBaker::Data::DataManager::Save(std::string path)
{
    std::cout << "Save " << " " << path << std::endl;
}

std::list<std::shared_ptr<LightmapBaker::Renderer::Mesh>> LightmapBaker::Data::DataManager::Load(const std::string& path)
{
    std::cout << "Load " << " " << path << std::endl;

    std::ifstream fout(path, std::ios::in | std::ios::binary);
    std::list<std::shared_ptr<LightmapBaker::Renderer::Mesh>> resultList;
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
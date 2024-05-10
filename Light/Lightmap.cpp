#include "Lightmap.hpp"
#include <assert.h>
#include "../Renderer/UI.hpp"

void LightmapBaker::Light::Lightmap::GetInputMesh(const std::vector<std::shared_ptr<Renderer::Mesh>>& meshList, Thekla::Atlas_Input_Mesh& inputMesh)
{
    using namespace Thekla;

    bakedMeshList = meshList;

    for (int n = 0; n < meshList.size(); ++n)
    {
        auto mesh = meshList[n];
        inputMesh.vertex_count += mesh->vertices.size();
        inputMesh.face_count += mesh->indices.size() / 3;
    }
    inputMesh.vertex_array = new Atlas_Input_Vertex[inputMesh.vertex_count];
    vertexColors.resize(inputMesh.vertex_count);
    int f = 0;
    int meshColorIndex = 0;
    for (int n = 0; n < meshList.size(); ++n)
    {
        auto mesh = meshList[n];
        for (int i = 0; i < mesh->vertices.size(); ++i)
        {
            inputMesh.vertex_array[f + i].position[0] = mesh->vertices[i].position.x;
            inputMesh.vertex_array[f + i].position[1] = mesh->vertices[i].position.y;
            inputMesh.vertex_array[f + i].position[2] = mesh->vertices[i].position.z;

            inputMesh.vertex_array[f + i].normal[0] = mesh->vertices[i].normal.x;
            inputMesh.vertex_array[f + i].normal[1] = mesh->vertices[i].normal.y;
            inputMesh.vertex_array[f + i].normal[2] = mesh->vertices[i].normal.z;

            inputMesh.vertex_array[f + i].uv[0] = mesh->vertices[i].uv.x;
            inputMesh.vertex_array[f + i].uv[1] = mesh->vertices[i].uv.y;

            vertexColors[f + i] = mesh->color;

            inputMesh.vertex_array[f + i].first_colocal = f + i;
            for (int vv = 0; vv < i; vv++)
            {
                if (inputMesh.vertex_array[f + i].position[0] == inputMesh.vertex_array[f + vv].position[0] &&
                    inputMesh.vertex_array[f + i].position[1] == inputMesh.vertex_array[f + vv].position[1] &&
                    inputMesh.vertex_array[f + i].position[2] == inputMesh.vertex_array[f + vv].position[2])
                {
                    inputMesh.vertex_array[f + i].first_colocal = f + vv;
                }
            }

            meshVertices.push_back(mesh->vertices[i]);
        }
         f += mesh->vertices.size();
    }

    inputMesh.face_array = new Atlas_Input_Face[inputMesh.face_count];
    f = 0;
    int vn = 0;
    for (int n = 0; n < meshList.size(); ++n)
    {
        auto mesh = meshList[n];
        for (int i = 0; i < mesh->indices.size() / 3; ++i)
        {
            inputMesh.face_array[f + i].material_index = 0;
            inputMesh.face_array[f + i].vertex_index[0] = mesh->indices[i * 3 + 0] + vn;
            inputMesh.face_array[f + i].vertex_index[1] = mesh->indices[i * 3 + 1] + vn;
            inputMesh.face_array[f + i].vertex_index[2] = mesh->indices[i * 3 + 2] + vn;
        }

        f += mesh->indices.size() / 3;
        vn += mesh->vertices.size();
    }
}

void LightmapBaker::Light::Lightmap::Bake(const std::shared_ptr<Renderer::Mesh>& mesh)
{
    using namespace Thekla;

    std::vector<std::shared_ptr<Renderer::Mesh>> meshList = { mesh };
    Atlas_Input_Mesh inputMesh = Atlas_Input_Mesh();
    GetInputMesh(meshList, inputMesh);

    // Generate Atlas_Output_Mesh.
    Atlas_Options atlasOptions;
    atlas_set_default_options(&atlasOptions);

    // Avoid brute force packing, since it can be unusably slow in some situations.
    atlasOptions.packer_options.witness.packing_quality = 1;

    Atlas_Error error = Atlas_Error_Success;
    outputMesh = atlas_generate(&inputMesh, &atlasOptions, &error);

    
    Renderer::LoggerUI::AddLog("Atlas mesh has " + std::to_string(outputMesh->vertex_count) + " verts");
    Renderer::LoggerUI::AddLog("Atlas mesh has " + std::to_string(outputMesh->index_count / 3) + " triangles");
}

void LightmapBaker::Light::Lightmap::Bake(const std::vector<std::shared_ptr<Renderer::Mesh>>& meshList)
{
    using namespace Thekla;

    Atlas_Input_Mesh inputMesh = Atlas_Input_Mesh();
    GetInputMesh(meshList, inputMesh);

    // Generate Atlas_Output_Mesh.
    Atlas_Options atlasOptions;
    atlas_set_default_options(&atlasOptions);

    // Avoid brute force packing, since it can be unusably slow in some situations.
    atlasOptions.packer_options.witness.packing_quality = 1;

    Atlas_Error error = Atlas_Error_Success;
    outputMesh = atlas_generate(&inputMesh, &atlasOptions, &error);

    Renderer::LoggerUI::AddLog("Atlas mesh has " + std::to_string(outputMesh->vertex_count) + " verts");
    Renderer::LoggerUI::AddLog("Atlas mesh has " + std::to_string(outputMesh->index_count / 3) + " triangles");
}

std::vector<std::shared_ptr<LightmapBaker::Renderer::Mesh>> LightmapBaker::Light::Lightmap::GetAtlasUVMesh()
{
    std::vector<std::shared_ptr<Renderer::Mesh>> meshList;
    for (int i = 0; i < outputMesh->vertex_count; ++i)
    {
        auto& vertex = outputMesh->vertex_array[i];
        int ref = vertex.xref;
        meshVertices[ref].uv.x = vertex.uv[0] / (float)outputMesh->atlas_width;
        meshVertices[ref].uv.y = vertex.uv[1] / (float)outputMesh->atlas_height;
    }

    int meshVerticesIndex = 0;
    for (auto& iter : bakedMeshList)
    {
        std::shared_ptr<Renderer::Mesh> mesh = std::make_shared<Renderer::Mesh>(*iter.get());
        mesh->color = glm::vec3(1.0f, 1.0f, 1.0f);
        mesh->CreateIndexBuffer();
        for (int i = 0; i < mesh->vertices.size(); ++i)
        {
            mesh->vertices[i] = meshVertices[meshVerticesIndex++];
        }

        meshList.push_back(mesh);
    }
    return meshList;
}

void LightmapBaker::Light::Lightmap::Destroy()
{
    atlas_free(outputMesh);
    vertexColors.clear();
    meshVertices.clear();
}

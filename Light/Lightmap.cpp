#include "Lightmap.hpp"
#include <assert.h>
#include <iostream>

void LightmapBaker::Light::Lightmap::GetInputMesh(const std::vector<std::shared_ptr<Renderer::Mesh>>& meshList, Thekla::Atlas_Input_Mesh& inputMesh)
{
    using namespace Thekla;

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

            meshVertices.push_back(&mesh->vertices[i]);
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

    std::cout << "Atlas mesh has " << outputMesh->vertex_count << " verts\n";
    std::cout << "Atlas mesh has " << outputMesh->index_count / 3 << " triangles\n";
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

    std::cout << "Atlas mesh has " << outputMesh->vertex_count << " verts\n";
    std::cout << "Atlas mesh has " << outputMesh->index_count / 3 << " triangles\n";
}

void LightmapBaker::Light::Lightmap::Destroy()
{
    atlas_free(outputMesh);
    vertexColors.clear();
    meshVertices.clear();
}

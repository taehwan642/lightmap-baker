#pragma once
#include <memory>
#include <vector>
#include <map>
#include "Mesh.hpp"
#include "RenderEnums.hpp"

namespace LightmapBaker
{
    namespace Renderer
    {
        class Splitter
        {
        private:
            SplitterType type = SplitterType::LEFT;
            int framebufferHeight = 0;
            int framebufferWidth = 0;
            int renderIndex = 0;
            std::map<int, std::vector<std::shared_ptr<Mesh>>> renderMeshList;
        private:
            Splitter();
        public:
            Splitter(SplitterType type);
            void Render();
            void Destroy();

            void SetFrameBufferWidthHeight(int width, int height);
            void SetRenderIndex(int index);
            void AddRenderMesh(const std::shared_ptr<Mesh>& mesh);
            void RemoveRenderMesh(const std::shared_ptr<Mesh>& mesh);
            void ClearRenderMesh();
        };
    }
}

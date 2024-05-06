#pragma once
#include "GLUT/glut.h"
#include "GLM/vec2.hpp"

namespace LightmapBaker
{
    namespace Renderer
    {
        class RenderTarget
        {
        public:
            glm::vec2 resolution;
            GLuint frameBuffer;
            GLuint renderTexture;
            GLuint depthBuffer;

            void Initialize(glm::vec2 resolution);
            static void BindDefault();
            void Bind();
            void Destroy();
        };
    }
}

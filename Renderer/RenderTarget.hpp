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
            RenderTarget(glm::vec2 resolution = glm::vec2(1280.0f, 720.0f));
            ~RenderTarget();

            glm::vec2 resolution;
            GLuint frameBuffer;
            GLuint renderTexture;
            GLuint depthBuffer;

            static void BindDefault();
            void Bind();
        };
    }
}

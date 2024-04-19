#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "RenderTarget.hpp"

void LightmapBaker::Renderer::RenderTarget::Initialize(glm::vec2 resolution)
{
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution.x, resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);

    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}


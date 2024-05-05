#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "RenderTarget.hpp"
#include "Renderer.hpp"
#include <iostream>

void LightmapBaker::Renderer::RenderTarget::Initialize(glm::vec2 resolution)
{
    this->resolution = resolution;
    glGenFramebuffers(1, &frameBuffer);
    glGenTextures(1, &renderTexture);
    glGenRenderbuffers(1, &depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resolution.x, resolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, resolution.x, resolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "FRAMEBUFFER INITIALIZE ISSUE" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void LightmapBaker::Renderer::RenderTarget::BindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Renderer::framebufferWidth, Renderer::framebufferHeight);
}

void LightmapBaker::Renderer::RenderTarget::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, resolution.x, resolution.y);
}

void LightmapBaker::Renderer::RenderTarget::Destroy()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &renderTexture);
    glDeleteRenderbuffers(1, &depthBuffer);
}


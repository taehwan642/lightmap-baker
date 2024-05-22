#include "Splitter.hpp"
#include "GLUT/glut.h"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "ToolState.hpp"

LightmapBaker::Renderer::Splitter::Splitter()
{
}

LightmapBaker::Renderer::Splitter::Splitter(SplitterType type) : type(type)
{
}

LightmapBaker::Renderer::Splitter::~Splitter()
{
    for (auto& iter : renderMeshList)
    {
        iter.second.clear();
    }
    renderMeshList.clear();
}

void LightmapBaker::Renderer::Splitter::Render()
{
    if (type == SplitterType::RIGHT)
    {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
	float splitX = ToolState::GetInstance().GetSplitXPosition();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, framebufferWidth, framebufferHeight, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_TEXTURE_2D);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    glDepthMask(GL_FALSE);
    glClear(GL_STENCIL_BUFFER_BIT);

    type == SplitterType::LEFT ? glColor3f(1, 0.3f, 0.3f) : glColor3f(0, 0.3f, 0.3f);;
    glBegin(GL_QUADS);
    if (type == SplitterType::LEFT)
    {
        glVertex2f(0, 0);
        glVertex2f(splitX, 0);
        glVertex2f(splitX, framebufferHeight);
        glVertex2f(0, framebufferHeight);
    }
    else if (type == SplitterType::RIGHT)
    {
        glVertex2f(splitX, 0);
        glVertex2f(framebufferWidth, 0);
        glVertex2f(framebufferWidth, framebufferHeight);
        glVertex2f(splitX, framebufferHeight);
    }

    glEnd();
    glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    Renderer& renderer = Renderer::GetInstance();
    MainCamera& camera = MainCamera::GetInstance();
    float aspectRatio = renderer.screenHeight / (float)renderer.screenWidth;
    gluPerspective(80, aspectRatio, 0.01f, camera.distance * 2 + 100);

    camera.Render();

    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDepthMask(GL_TRUE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    for (int i = 0; i < renderMeshList[renderIndex].size(); ++i)
    {
        renderMeshList[renderIndex][i]->Render({ ToolState::GetInstance().GetMeshDrawMode() });
    }

    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

}

void LightmapBaker::Renderer::Splitter::SetFrameBufferWidthHeight(int width, int height)
{
    framebufferWidth = width;
    framebufferHeight = height;
}

void LightmapBaker::Renderer::Splitter::SetRenderIndex(int index)
{
    renderIndex = index;
}

void LightmapBaker::Renderer::Splitter::AddRenderMesh(const std::shared_ptr<Mesh>& mesh)
{
    if (renderMeshList.find(renderIndex) == renderMeshList.end()) renderMeshList.emplace(renderIndex, std::vector<std::shared_ptr<Mesh>>());
    renderMeshList[renderIndex].push_back(mesh);
}

void LightmapBaker::Renderer::Splitter::RemoveRenderMesh(const std::shared_ptr<Mesh>& mesh)
{
    for (std::vector<std::shared_ptr<Mesh>>::iterator iter = renderMeshList[renderIndex].begin(); iter != renderMeshList[renderIndex].end();)
    {
        if (iter->get() == mesh.get())
        {
            iter = renderMeshList[renderIndex].erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void LightmapBaker::Renderer::Splitter::ClearRenderMesh()
{
    renderMeshList[renderIndex].clear();
}

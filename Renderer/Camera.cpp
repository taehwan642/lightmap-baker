#include "Camera.hpp"
#include "GLUT/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>

void LightmapBaker::Renderer::Camera::Render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = (distance * -sinf(angle.x * (M_PI / 180)) * cosf((angle.y) * (M_PI / 180))) + position.x;
    float camY = (distance * -sinf((angle.y) * (M_PI / 180))) + position.y;
    float camZ = (-distance * cosf((angle.x) * (M_PI / 180)) * cosf((angle.y) * (M_PI / 180))) + position.z;
    gluLookAt(camX, camY, camZ,
        position.x, position.y, position.z,
        0.0, 1.0, 0.0);
}

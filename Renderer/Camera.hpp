#pragma once
#include "GLM/vec2.hpp"
#include "GLM/vec3.hpp"
#include "../Utility/Singleton.hpp"

namespace LightmapBaker
{
    namespace Renderer
    {
        class Camera
        {
        public:
            glm::vec3 position = glm::vec3(0, 0, 0);
            glm::vec2 angle = glm::vec2(0, 0);
            float distance = 500.0f;
            float speed = 1.0f;

        public:
            void Render();
        };

        class MainCamera : public Camera, public Utility::Singleton<MainCamera>
        {
        private:
            MainCamera() = default;
            ~MainCamera() = default;
            friend class Utility::Singleton<MainCamera>;
        };
    }
}

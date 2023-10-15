#pragma once

#include "MouseEvent.h"
#include "ApplicationEvent.h"
#include "Timestep.h"
#include "Renderer/OrthographicCamera.h"

namespace Hazel {
    class OrthographicCameraController {
    public:
        OrthographicCameraController(float aspecRato, bool rotation = false);
        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera(){return m_Camera;}
        const OrthographicCamera& GetCamera() const {return m_Camera;}

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowsResized(WindowResizeEvent& e);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        bool m_Rotation = false;
        glm::vec3 m_CameraPosition;
        float m_CameraRotation = 0.0f;
        float m_CameraMoveSpeed = 5.f;
        float m_CameraRotationSpeed = 180.0f;
        OrthographicCamera m_Camera;
    };

}

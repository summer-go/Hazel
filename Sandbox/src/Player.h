#pragma once

#include "Timestep.h"
#include "ParticleSystem.h"
#include <glm/gtc/matrix_transform.hpp>

class Player {
public:
    Player();

    void LoadAssets();

    void OnUpdate(Hazel::Timestep timestep);
    void OnRender();

    void OnImGuiRender();

    void Reset();

    float GetRotation(){return m_Velocity.y * 4.0f - 90.0;}
    const glm::vec2& GetPosition() const { return m_Position;}
    uint32_t GetScore() const { return (uint32_t)(m_Position.x + 10.f) / 10.0f; };
private:
    glm::vec2 m_Position = {-10.0f, 0.0f};
    glm::vec2 m_Velocity = {5.0f, 0.0f};

    float m_EnginePower = 0.5f;
    float m_Gravity = 0.4f;

    float m_Time = 0.0f;
    float m_SmokeEmitInterval = 0.4f;
    float m_SmokeNextEmitTime = m_SmokeEmitInterval;

    ParticleProps m_SmokeParticle, m_EngineParticle;
    ParticleSystem m_ParticleSystem;

    Hazel::Ref<Hazel::Texture2D> m_ShipTexture;
};


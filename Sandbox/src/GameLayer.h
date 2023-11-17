//
// Created by ByteDance on 2023/11/16.
//

#pragma once

#include "Hazel.h"
#include "Level.h"
#include <imgui.h>

class GameLayer : public Hazel::Layer{
public:
    GameLayer();

    ~GameLayer() override = default;

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;

    void OnImGuiRender() override;

    void OnEvent(Hazel::Event &event) override;

    bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& e);
    bool OnWindowResize(Hazel::WindowResizeEvent& e);

private:
    void CreateCamera(uint32_t width, uint32_t height);

private:
    Hazel::Scope<Hazel::OrthographicCamera> m_Camera;
    Level m_Level;
    ImFont* m_Font{};
    float m_Time = 0.0f;
    bool m_Blink = false;

    enum class GameState
    {
        Play = 0, MainMenu = 1, GameOver = 2
    };

    GameState m_State = GameState::MainMenu;
};
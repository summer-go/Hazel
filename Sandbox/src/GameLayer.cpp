//
// Created by ByteDance on 2023/11/16.
//

#include "GameLayer.h"
#include "Random.h"
#include "Hazel/Core/Core.h"

using namespace Hazel;
GameLayer::GameLayer()
    : Layer("GameLayer")
{
    auto& window = Application::Get().GetWindow();
    CreateCamera(window.GetWidth(), window.GetHeight());

    Random::Init();
}

void GameLayer::OnAttach() {
    m_Level.Init();

    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("../assets/OpenSans-Regular.ttf", 120.0f);
}

void GameLayer::OnDetach() {
}

void GameLayer::OnUpdate(Hazel::Timestep ts) {
    m_Time += ts;
    if ((int)(m_Time * 10.0f) % 8 > 4) {
        m_Blink = !m_Blink;
    }

    if (m_Level.IsGameOver()) {
        HZ_INFO("GameLayer::OnUpdate IsGameOver: true");
        m_State = GameState::GameOver;
    }

    const auto& playerPos = m_Level.GetPlayer().GetPosition();
    m_Camera->SetPosition({playerPos.x, playerPos.y, 0.0f});

    switch (m_State) {
        case GameState::Play:
        {
            m_Level.OnUpdate(ts);
            break;
        }
    }

    // Render
    Hazel::RenderCommand::SetClearColor({0.0f, 0.0f, 0.0f, 1});
    Hazel::RenderCommand::Clear();

    Hazel::Renderer2D::BeginScene(*m_Camera);
    m_Level.OnRender();
    Hazel::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender() {
    switch (m_State) {
        case GameState::Play:
        {
            HZ_INFO("GameLayer::OnImGuiRender Play");
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
            break;
        }
        case GameState::MainMenu:
        {
            HZ_INFO("GameLayer::OnImGuiRender MainMenu");
            auto pos = ImGui::GetWindowPos();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink) {
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
            }
            break;
        }
        case GameState::GameOver:
        {
            HZ_INFO("GameLayer::OnImGuiRender GameOver");
            auto pos = ImGui::GetWindowPos();
            auto width = Application::Get().GetWindow().GetWidth();
            auto height = Application::Get().GetWindow().GetHeight();
            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink) {
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
            }

            pos.x += 200.0f;
            pos.y += 150.0f;
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: ") + std::to_string(playerScore);
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, pos, 0xffffffff, scoreStr.c_str());
            break;
        }
    }
    m_Level.OnImGuiRender();
}

void GameLayer::OnEvent(Hazel::Event &e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(GameLayer::OnWindowResize));
    dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(MouseButtonPressedEvent &e) {
    if(m_State == GameState::GameOver) {
        m_Level.Reset();
    }

    m_State = GameState::Play;
    return false;
}

bool GameLayer::OnWindowResize(WindowResizeEvent &e) {
    CreateCamera(e.GetWidth(), e.GetHeight());
    return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height) {
    float aspecRatio  = (float)width / (float)height;

    float camWidth = 8.0f;
    float bottom = -camWidth;
    float top = camWidth;
    float left = bottom * aspecRatio;
    float right = top * aspecRatio;
    m_Camera = CreateScope<OrthographicCamera>(left, right, bottom ,top);
}

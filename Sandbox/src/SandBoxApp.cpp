//
// Created by baidu on 2023/9/1.
//

//#include "Test.h"
#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
    ExampleLayer() : Layer("Example") {

    }

    void OnUpdate() override{
//        HZ_INFO("ExampleLayer::Update");
        if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB)) {
            HZ_TRACE("TAB key is pressed(poll)!");
        }
    }

    void OnEvent(Hazel::Event& event) override {
        if (event.GetEventType() == Hazel::EventType::KeyPressed) {
            auto& e = dynamic_cast<Hazel::KeyPressedEvent&>(event);
            if (e.GetKeyCode() == HZ_KEY_TAB) {
                HZ_TRACE("Tab key is pressed(event)!");
            }
            HZ_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
};

class Sandbox : public Hazel::Application {
public:
    Sandbox(){
        PushOverlay(new ExampleLayer());
        PushOverlay(new Hazel::ImGuiLayer());
    }

    ~Sandbox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication() {
    return new Sandbox();
}
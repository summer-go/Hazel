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
        HZ_INFO("ExampleLayer::Update");
    }

    void OnEvent(Hazel::Event& event) override {
        HZ_TRACE("ExampleLayer::OnEvent {0}", event.ToString());
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
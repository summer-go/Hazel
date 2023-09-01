//
// Created by baidu on 2023/9/1.
//

//#include "Test.h"
#include <Hazel.h>
class Sandbox : public Hazel::Application {
public:
    Sandbox(){

    }

    ~Sandbox()
    {

    }
};

Hazel::Application* Hazel::CreateApplication() {
    return new Sandbox();
}
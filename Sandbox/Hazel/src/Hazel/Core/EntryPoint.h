//
// Created by baidu on 2023/9/2.
//

#ifndef SANBOX_ENTRYPOINT_H
#define SANBOX_ENTRYPOINT_H

extern Hazel::Application* Hazel::CreateApplication();


int main(int argc, char** argv) {
    Hazel::Log::Init();
//    Hazel::Log::GetCoreLogger()->warn("Initialized Log!");
//    Hazel::Log::GetClientLogger()->info("Hello!");
    HZ_CORE_WARN("Initialed Log!!!!");
    int a = 5;
    HZ_INFO("Hello! Var={0}", a);

    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
}
#endif //SANBOX_ENTRYPOINT_H

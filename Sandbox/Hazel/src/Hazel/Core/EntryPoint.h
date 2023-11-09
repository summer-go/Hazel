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

    HZ_INFO("main  startup----");
    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
    auto app = Hazel::CreateApplication();
    HZ_PROFILE_END_SESSION();

    HZ_INFO("main  before Runtime----");

    HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->Run();
    HZ_PROFILE_END_SESSION();

    HZ_INFO("main  after Runtime----");


    HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Shutdown.json");
    delete app;
    HZ_PROFILE_END_SESSION();

    HZ_INFO("main  exit ----");

}
#endif //SANBOX_ENTRYPOINT_H

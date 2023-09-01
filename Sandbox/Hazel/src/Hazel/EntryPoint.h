//
// Created by baidu on 2023/9/2.
//

#ifndef SANBOX_ENTRYPOINT_H
#define SANBOX_ENTRYPOINT_H

#ifdef HZ_PLATFORM_WINDOWS
extern Hazel::Application* Hazel::CreateApplication();


int main(int argc, char** argv) {

    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
}
#endif
#endif //SANBOX_ENTRYPOINT_H

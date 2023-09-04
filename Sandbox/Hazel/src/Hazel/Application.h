//
// Created by baidu on 2023/9/2.
//

#ifndef SANBOX_APPLICATION_H
#define SANBOX_APPLICATION_H

#include "Event.h"
#include <memory>
#include "Window.h"
namespace Hazel{
class Application {
public:
    Application();
    virtual ~Application();
    void Run();

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
};

    Application* CreateApplication();
}



#endif //SANBOX_APPLICATION_H

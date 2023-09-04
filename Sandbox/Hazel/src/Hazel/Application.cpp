//
// Created by baidu on 2023/9/2.
//

#include "Application.h"
#include "ApplicationEvent.h"
#include "Log.h"

namespace Hazel{
    Application::Application() {

    }

    Application::~Application() {

    }

    void Application::Run() {
        WindowResizeEvent e(1200, 720);
        if (e.IsInCategory(EventCategoryApplication)) {
            HZ_TRACE(e.ToString());
        }

        if (e.IsInCategory(EventCategoryInput)) {
            HZ_TRACE(e.ToString());
        }

        while(true) {

        }
    }

}
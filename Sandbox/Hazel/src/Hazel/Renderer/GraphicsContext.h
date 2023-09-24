//
// Created by xiatian05 on 2023/9/24.
//

#pragma once

namespace Hazel {
    class GraphicsContext {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}
//
// Created by baidu on 2023/9/2.
//

#pragma once

#include <memory>

#ifdef HZ_PLATFORM_WINDOWS
    #ifdef HZ_BUILD_DLL
        #define HAZEL_API __declspec(dllexport)
    #else
        #define HAZEL_API __declspec(dllimport)
    #endif
#else
//    #error Hazel only support Windows!

#endif

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Hazel {
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

//
// Created by baidu on 2023/9/2.
//

#ifndef SANBOX_CORE_H
#define SANBOX_CORE_H


#ifdef HZ_PLATFORM_WINDOWS
    #ifdef HZ_BUILD_DLL
        #define HAZEL_API __declspec(dllexport)
    #else
        #define HAZEL_API __declspec(dllimport)
    #endif
#else
//    #error Hazel only support Windows!

#endif
#endif //SANBOX_CORE_H

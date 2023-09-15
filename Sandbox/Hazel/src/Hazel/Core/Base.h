//
// Created by baidu on 2023/9/4.
//

#ifndef SANBOX_BASE_H
#define SANBOX_BASE_H

#include "Log.h"
#ifdef __APPLE__
#include <assert.h>
#endif


#define HZ_ENABLE_ASSERTS
#define BIT(x) (1 << x)


#ifdef HZ_ENABLE_ASSERTS

// mac端assert,其他平台的assert需要其他平台实现
#ifdef __APPLE__
#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__);assert(x); } }
#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); assert(x); } }
#else
#define HZ_ASSERT(x, ...)
#define HZ_CORE_ASSERT(x, ...)
#endif

#else
#define HZ_ASSERT(x, ...)
#define HZ_CORE_ASSERT(x, ...)
#endif

#endif //SANBOX_BASE_H

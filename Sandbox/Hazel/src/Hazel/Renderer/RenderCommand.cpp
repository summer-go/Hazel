//
// Created by xiatian05 on 2023/10/6.
//

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}
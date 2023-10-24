//
// Created by baidu on 2023/9/2.
//

#ifndef SANBOX_HAZEL_H
#define SANBOX_HAZEL_H

#include <stdio.h>
#include "Hazel/Core/Application.h"
#include "Hazel/Core/Log.h"
#include "Layer.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"
#include "Hazel/Events/KeyEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"



// -----Renderer----------
#include <Renderer/RenderCommand.h>
#include <Renderer/Renderer.h>
#include <Renderer/OrthographicCamera.h>
#include <Renderer/Buffer.h>
#include <Renderer/Shader.h>
#include <Renderer/VertexArray.h>
#include <Hazel/Core/Timestep.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Hazel/Core/Core.h"
#include <Renderer/Texture.h>
#include "OrthographicCameraController.h"


// -----Entry Point-------
//#include "Hazel/Core/EntryPoint.h"

#endif //SANBOX_HAZEL_H

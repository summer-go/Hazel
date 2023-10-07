//
// Created by baidu on 2023/9/7.
//

#pragma once
#include <string>
#include "Event.h"
#include "Timestep.h"

namespace Hazel {
    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach(){}
        virtual void OnDetach(){}
        virtual void OnUpdate(Timestep ts){}
        virtual void OnImGuiRender(){}
        virtual void OnEvent(Event& event){}

        inline const std::string& GetName() const {return m_DebugName;}

    protected:
        std::string m_DebugName;
    };
}


//
// Created by baidu on 2023/9/7.
//

#pragma once
#include <string>
#include "Event.h"
namespace Hazel {
    class Layer {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach(){}
        virtual void OnDetach(){}
        virtual void OnUpdate(){}
        virtual void OnEvent(Event& event){}

        inline const std::string& GetName() const {return m_DebugName;}

    protected:
        std::string m_DebugName;
    };
}


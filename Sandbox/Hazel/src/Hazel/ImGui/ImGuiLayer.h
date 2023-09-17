#pragma once

#include "Layer.h"
namespace Hazel {
class ImGuiLayer : public Layer{
public:
    ImGuiLayer();
    ~ImGuiLayer() override;

   virtual void OnAttach() override;
   virtual void OnDetach() override;
   virtual void OnImGuiRender() override;

   void Begin();
   void End();
private:
    float m_Time = 0.0f;
};
}
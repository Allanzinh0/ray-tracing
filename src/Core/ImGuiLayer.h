#pragma once

#include "Window.h"

class ImGuiLayer
{
public:
    ImGuiLayer(const Window& window);
    ~ImGuiLayer();

    void BeginFrame();
    void Swap();
private:
    bool m_DockspaceOpen = true;
};

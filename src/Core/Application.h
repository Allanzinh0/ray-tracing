#pragma once

#include "Window.h"
#include "ImGuiLayer.h"
#include "Screen.h"

class Application
{
public:
    Application(uint32_t width, uint32_t height, const std::string& title);
    ~Application();

    void Run();
    
    static Application& Get();
private:
    Window m_Window;
    ImGuiLayer m_ImGuiLayer;
    Screen m_Screen;
};

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
    double GetDeltaTime() { return m_DeltaTime; }

    Window& GetWindow() { return m_Window; }
    
    static Application& Get();
private:
    Window m_Window;
    ImGuiLayer m_ImGuiLayer;
    Screen m_Screen;
    double m_Time = 0.0;
    double m_DeltaTime = 0.0;
    double m_LastTime = 0.0;
};

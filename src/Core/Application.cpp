#include "Application.h"

static Application* s_Instance = nullptr;

Application& Application::Get()
{
    return *s_Instance;
}

Application::Application(uint32_t width, uint32_t height, const std::string& title)
    : m_Window(width, height, title), m_ImGuiLayer(m_Window)
{
    s_Instance = this;
}

Application::~Application()
{
    s_Instance = nullptr;
}

void Application::Run()
{
    while (!m_Window.WindowRunning())
    {
        m_Window.BeginFrame();
        m_ImGuiLayer.BeginFrame();

        m_Screen.OnUIRender();

        m_ImGuiLayer.Swap();
        m_Window.Swap();
    }
}
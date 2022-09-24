#pragma once

#include <stdint.h>
#include <string>

#include <GLFW/glfw3.h> 

class Window
{
public:
    Window(uint32_t width, uint32_t height, const std::string& title);
    ~Window();

    void BeginFrame();
    void Swap();
    
    double GetTime() { return glfwGetTime(); }

    GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }
    bool WindowRunning() const { return glfwWindowShouldClose(m_WindowHandle); }
private:
    int Init(uint32_t width, uint32_t height, const std::string& title);
private:
    GLFWwindow* m_WindowHandle = nullptr;
    int32_t m_Width = 0;
    int32_t m_Height = 0;
};
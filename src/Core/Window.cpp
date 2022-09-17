#include "Window.h"

#include <glad.h>

Window::Window(uint32_t width, uint32_t height, const std::string& title)
{
    Init(width, height, title);
}

int Window::Init(uint32_t width, uint32_t height, const std::string& title)
{
    if (!glfwInit())
        return -1;
    
    m_WindowHandle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!m_WindowHandle)
        return -1;

    glfwMakeContextCurrent(m_WindowHandle);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    return 0;
}

Window::~Window()
{
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();
}

void Window::BeginFrame()
{
    glfwPollEvents();
    glfwGetFramebufferSize(m_WindowHandle, &m_Width, &m_Height);
    glViewport(0, 0, m_Width, m_Height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::Swap()
{
    glfwSwapBuffers(m_WindowHandle);
}
#include "Input.h"

#include "../Core/Application.h"

#include <GLFW/glfw3.h>

bool Input::IsKeyDown(KeyCode keycode)
{
    GLFWwindow* windowHandle = Application::Get().GetWindow().GetWindowHandle();
    int state = glfwGetKey(windowHandle, (int)keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonDown(MouseButton button)
{
    GLFWwindow* windowHandle = Application::Get().GetWindow().GetWindowHandle();
    int state = glfwGetMouseButton(windowHandle, (int)button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
    GLFWwindow* windowHandle = Application::Get().GetWindow().GetWindowHandle();

    double x, y;
    glfwGetCursorPos(windowHandle, &x, &y);
    return { (float)x, (float)y };
}

void Input::SetCursorMode(CursorMode mode)
{
    GLFWwindow* windowHandle = Application::Get().GetWindow().GetWindowHandle();
    glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
}
#pragma once

#include <memory>
#include <stdint.h>

#include "../Image.h"
#include "../Renderer.h"
#include "../Camera.h"

class Screen
{
public:
    Screen() : m_Camera(60.0f, 0.1f, 100.0f) {}
    ~Screen() = default;

    void OnUIRender();
private:
    void Render();
private:
    Renderer m_Renderer;
    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;
    float m_LastRenderTime = 0.0f;
    Camera m_Camera;
};
#pragma once

#include <memory>
#include <stdint.h>

#include "Image.h"
#include "Renderer.h"

class Screen
{
public:
    Screen() = default;
    ~Screen() = default;

    void OnUIRender();
private:
    void Render();
private:
    Renderer m_Renderer;
    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;
    float m_LastRenderTime = 0.0f;
};
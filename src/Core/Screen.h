#pragma once

#include <memory>
#include <stdint.h>

#include "../Image.h"
#include "../Renderer.h"
#include "../Camera.h"
#include "../Scene.h"

class Screen
{
public:
    Screen();
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
    Scene m_Scene;
};
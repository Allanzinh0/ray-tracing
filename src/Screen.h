#pragma once

#include <memory>
#include <stdint.h>

#include "Image.h"

class Screen
{
private:
    /* data */
public:
    Screen() = default;
    ~Screen() = default;

    void OnUIRender();
private:
    void Render();
private:
    std::shared_ptr<Image> m_Image;
    uint32_t* m_ImageData = nullptr;
    uint32_t m_ViewportWidth = 0;
    uint32_t m_ViewportHeight = 0;
    float m_LastRenderTime = 0.0f;
};
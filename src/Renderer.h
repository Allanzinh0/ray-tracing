#pragma once

#include <memory>
#include <stdint.h>

#include "glm/glm.hpp"

#include "Image.h"

class Renderer
{
public:
    Renderer() = default;
    ~Renderer() = default;

    void Render();
    void OnResize(uint32_t width, uint32_t height);

    std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; }
private:
    uint32_t PerPixel(glm::vec2 coord);

private:
    std::shared_ptr<Image> m_FinalImage;
    uint32_t m_Width, m_Height;
    uint32_t* m_ImageData = nullptr;
};
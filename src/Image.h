#pragma once

#include <stdint.h>

class Image
{
private:
    uint32_t m_Width;
    uint32_t m_Height;
    uint32_t m_RendererID = 0;
public:
    Image(uint32_t width, uint32_t height, void* data);
    ~Image();

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }
    uint32_t GetRendererID() const { return m_RendererID; }

    void SetData(void* data);
};
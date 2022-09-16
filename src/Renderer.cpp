#include "Renderer.h"

void Renderer::Render()
{
    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            glm::vec2 coord = { x / (float)m_FinalImage->GetWidth(), y / (float)m_FinalImage->GetHeight() };
            coord = coord * 2.0f - 1.0f; // -1 -> +1
            m_ImageData[y * m_FinalImage->GetWidth() + x] = PerPixel(coord);
        }
    }

    m_FinalImage->SetData(m_ImageData);
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
    if (!m_FinalImage || width != m_FinalImage->GetWidth() || height != m_FinalImage->GetHeight())
    {
        m_FinalImage = std::make_shared<Image>(width, height, nullptr);
        m_Width = width;
        m_Height = height;

        delete[] m_ImageData;
        m_ImageData = new uint32_t[width * height];
    }
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
    glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);
    glm::vec3 rayDirection(coord.x * (m_Width / (float)m_Height), coord.y, -1.0f);

    glm::vec3 lightDir(1, 1, -1);
    lightDir = glm::normalize(lightDir);

    static float radius = 0.5f;

    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * glm::dot(rayOrigin, rayDirection);
    float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant >= 0.0f)
    {
        float t0 = (-b - glm::sqrt(discriminant)) / (2 * a);
        float t1 = (-b + glm::sqrt(discriminant)) / (2 * a);
        float t = (t0 < t1) ? t0 : t1;

        glm::vec3 hitPosition = rayOrigin + rayDirection * t;
        glm::vec3 normal = hitPosition - glm::vec3(0.0f);
        normal = glm::normalize(normal);

        glm::vec3 sphereColor(glm::dot(normal, -lightDir));

        uint32_t r = (uint8_t)((sphereColor.x * 0.5f + 0.5f) * 256);
        uint32_t g = (uint8_t)((sphereColor.y * 0.5f + 0.5f) * 256);
        uint32_t b = (uint8_t)((sphereColor.z * 0.5f + 0.5f) * 256);

        return 0xff000000 | (b << 16) | (g << 8) | r;
    } else 
        return 0xff202020;
}
#include "Renderer.h"

static uint32_t ConvertToRGBA(glm::vec4 color)
{
    uint32_t r = (uint8_t)(color.r * 255.0f);
    uint32_t g = (uint8_t)(color.g * 255.0f);
    uint32_t b = (uint8_t)(color.b * 255.0f);
    uint32_t a = (uint8_t)(color.a * 255.0f);

    return (a << 24) | (b << 16) | (g << 8) | r;
}

void Renderer::Render(const Camera& cam)
{
    Ray ray;
    ray.Origin = cam.GetPosition();

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            ray.Direction = cam.GetRayDirections()[y * m_FinalImage->GetWidth() + x];

            glm::vec4 color = TraceRay(ray);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_ImageData[y * m_FinalImage->GetWidth() + x] = ConvertToRGBA(color);
        }
    }

    m_FinalImage->SetData(m_ImageData);
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
    if (!m_FinalImage || width != m_FinalImage->GetWidth() || height != m_FinalImage->GetHeight())
    {
        m_FinalImage = std::make_shared<Image>(width, height, nullptr);

        delete[] m_ImageData;
        m_ImageData = new uint32_t[width * height];
    }
}

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
    glm::vec3 lightDir = glm::normalize(glm::vec3(-1,-1,-1));

    float radius = 0.5f;

    float a = glm::dot(ray.Direction, ray.Direction);
    float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
    float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0.0f)
        return glm::vec4(0.2f,0.2f,0.2f,1.0f);

    // float t0 = (-b + glm::sqrt(discriminant)) / (2 * a);
    float closestT = (-b - glm::sqrt(discriminant)) / (2 * a);

    glm::vec3 hitPosition = ray.Origin + ray.Direction * closestT;
    glm::vec3 normal = glm::normalize(hitPosition - glm::vec3(0.0f));

    // Light Intensity
    float d = glm::max(glm::dot(normal, -lightDir), 0.0f); // cos(angle)

    glm::vec3 sphereColor(0, 1, 1);
    sphereColor *= d;

    return glm::vec4(sphereColor, 1.0f);
}
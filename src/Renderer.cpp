#include "Renderer.h"

static uint32_t ConvertToRGBA(glm::vec4 color)
{
    uint32_t r = (uint8_t)(color.r * 255.0f);
    uint32_t g = (uint8_t)(color.g * 255.0f);
    uint32_t b = (uint8_t)(color.b * 255.0f);
    uint32_t a = (uint8_t)(color.a * 255.0f);

    return (a << 24) | (b << 16) | (g << 8) | r;
}

void Renderer::Render(const Scene& scene, const Camera& cam)
{
    Ray ray;
    ray.Origin = cam.GetPosition();

    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            ray.Direction = cam.GetRayDirections()[y * m_FinalImage->GetWidth() + x];

            glm::vec4 color = TraceRay(scene, ray);
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

glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
{
    float hitDistance = std::numeric_limits<float>::max();
    const Sphere* closestSphere = nullptr;

    for (const Sphere& sphere : scene.Spheres)
    {
        glm::vec3 origin = ray.Origin - sphere.Position;

        float a = glm::dot(ray.Direction, ray.Direction);
        float b = 2.0f * glm::dot(origin, ray.Direction);
        float c = glm::dot(origin, origin) - sphere.Radius * sphere.Radius;

        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0.0f)
            continue;

        float closestT = (-b - glm::sqrt(discriminant)) / (2 * a);

        if (closestT > 0.0f && closestT < hitDistance)
        {
            hitDistance = closestT;
            closestSphere = &sphere;
        }
    }

    if (closestSphere == nullptr)
    {
        float t = 0.5f * (glm::normalize(ray.Direction).y + 1);
        glm::vec3 skyColor = (1 - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
        return glm::vec4(skyColor,1.0f);
    }

    glm::vec3 origin = ray.Origin - closestSphere->Position;
    glm::vec3 hitPoint = origin + ray.Direction * hitDistance;
    glm::vec3 normal = glm::normalize(hitPoint);

    glm::vec3 lightDir = glm::normalize(glm::vec3(-1,-1,-1));
    float d = glm::max(glm::dot(normal, -lightDir), 0.0f); // cos(angle)

    glm::vec3 outColor = closestSphere->Albedo * d;

    return glm::vec4(outColor, 1.0f);
}
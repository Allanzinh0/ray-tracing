#include "Renderer.h"
#include "Random.h"

namespace Utils
{
    static uint32_t ConvertToRGBA(glm::vec4 color)
    {
        uint32_t r = (uint8_t)(color.r * 255.0f);
        uint32_t g = (uint8_t)(color.g * 255.0f);
        uint32_t b = (uint8_t)(color.b * 255.0f);
        uint32_t a = (uint8_t)(color.a * 255.0f);

        return (a << 24) | (b << 16) | (g << 8) | r;
    }
}

void Renderer::Render(const Scene& scene, const Camera& cam)
{
    m_ActiveScene = &scene;
    m_ActiveCamera = &cam;


    for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
    {
        for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
        {
            glm::vec4 color = PerPixel(x, y);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            m_ImageData[y * m_FinalImage->GetWidth() + x] = Utils::ConvertToRGBA(color);
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

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
    Ray ray;
    ray.Origin = m_ActiveCamera->GetPosition();
    ray.Direction = m_ActiveCamera->GetRayDirections()[y * m_FinalImage->GetWidth() + x];

    glm::vec3 color(0.0f);

    float multiplier = 1.0f;

    for (int i = 0; i < Bounces; i++)
    {
        Renderer::HitPayload payload = TraceRay(ray);

        if (payload.HitDistance < 0.0f)
        {
            glm::vec3 skyColor = glm::vec3(0.6f, 0.7f, 0.9f);

            color += skyColor * multiplier;
            break;
        }

        glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));
        float d = glm::max(glm::dot(payload.WorldNormal, -lightDir), 0.0f); // cos(angle)

        const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
        const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];
        glm::vec3 sphereColor = material.Albedo;
        sphereColor *= d;

        color += sphereColor * multiplier;

        multiplier *= 0.7f;

        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001f;
        ray.Direction = glm::reflect(ray.Direction, payload.WorldNormal + material.Roughness * Random::Vec3(-0.5f, 0.5f));
    }


    return glm::vec4(color, 1.0f);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
    Renderer::HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;

    const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];

    glm::vec3 origin = ray.Origin - closestSphere.Position;
    payload.WorldPosition = origin + ray.Direction * hitDistance;
    payload.WorldNormal = glm::normalize(payload.WorldPosition);

    payload.WorldPosition += closestSphere.Position;

    return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
    Renderer::HitPayload payload;
    payload.HitDistance = -1.0f;
    payload.WorldNormal = glm::normalize(ray.Direction);

    return payload;
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
    float hitDistance = std::numeric_limits<float>::max();
    int closestSphere = -1;

    for (size_t i = 0; i < m_ActiveScene->Spheres.size(); i++)
    {
        const Sphere& sphere = m_ActiveScene->Spheres[i];

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
            closestSphere = i;
        }
    }

    if (closestSphere < 0)
        return Miss(ray);

    return ClosestHit(ray, hitDistance, closestSphere);
}
#pragma once

#include <memory>
#include <stdint.h>

#include "glm/glm.hpp"

#include "Image.h"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"

class Renderer
{
public:
    struct Settings
    {
        bool Accumulate = true;
        int Bounces = 5;
    };
public:
    Renderer() = default;
    ~Renderer() = default;

    void Render(const Scene& scene, const Camera& cam);
    void OnResize(uint32_t width, uint32_t height);

    std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; }

    void ResetFrameIndex() { m_FrameIndex = 1; }
    Settings& GetSettings() { return m_Settings; }
private:
    struct HitPayload
    {
        float HitDistance;

        glm::vec3 WorldPosition;
        glm::vec3 WorldNormal;

        int ObjectIndex;
    };

    glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen
    
    HitPayload TraceRay(const Ray& ray);
    HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
    HitPayload Miss(const Ray& ray);

private:
    std::shared_ptr<Image> m_FinalImage;
    Settings m_Settings;

    const Scene* m_ActiveScene = nullptr;
    const Camera* m_ActiveCamera = nullptr;

    uint32_t* m_ImageData = nullptr;
    glm::vec4* m_AccumulationData = nullptr;

    uint32_t m_FrameIndex = 1;
};
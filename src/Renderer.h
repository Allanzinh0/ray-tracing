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
    Renderer() = default;
    ~Renderer() = default;

    void Render(const Scene& scene, const Camera& cam);
    void OnResize(uint32_t width, uint32_t height);

    std::shared_ptr<Image> GetFinalImage() const { return m_FinalImage; }
private:
    glm::vec4 TraceRay(const Scene& scene, const Ray& ray);

private:
    std::shared_ptr<Image> m_FinalImage;
    uint32_t* m_ImageData = nullptr;
};
#include "Screen.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"
#include "Application.h"

Screen::Screen()
    : m_Camera(60.0f, 0.1f, 100.0f)
{
    Material& pinkSphere = m_Scene.Materials.emplace_back();
    pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
    pinkSphere.Roughness = 0.0f;

    Material& blueSphere = m_Scene.Materials.emplace_back();
    blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
    blueSphere.Roughness = 0.1f;

    {
        Sphere sphere;
        sphere.Position = { 0.0f, 0.0f, 0.0f };
        sphere.Radius = 1.0f;
        sphere.MaterialIndex = 0;
        m_Scene.Spheres.push_back(sphere);
    }
    {
        Sphere sphere;
        sphere.Position = { 0.0f, -101.0f, 0.0f };
        sphere.Radius = 100.0f;
        sphere.MaterialIndex = 1;
        m_Scene.Spheres.push_back(sphere);
    }
}

void Screen::OnUIRender()
{
    m_Camera.OnUpdate(Application::Get().GetDeltaTime());
    Render();
    ImGui::Begin("Settings");
    ImGui::DragInt("Bounces", &m_Renderer.Bounces, 1, 1, 500);
    ImGui::Text("Last render: %.3fms", m_LastRenderTime);
    ImGui::End();

    ImGui::Begin("Spheres");

    for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
    {
        ImGui::PushID(i);

        Sphere& sphere = m_Scene.Spheres[i];
        if (ImGui::TreeNode("Sphere"))
        {
            ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
            ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
            ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);
            ImGui::TreePop();
            ImGui::Separator();
        }

        ImGui::PopID();
    }

    ImGui::End();

    ImGui::Begin("Materials");

    for (size_t i = 0; i < m_Scene.Materials.size(); i++)
    {
        ImGui::PushID(i);

        Material& material = m_Scene.Materials[i];
        if (ImGui::TreeNode("Material"))
        {
            ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo), 0.1f);
            ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
            ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
            ImGui::TreePop();
            ImGui::Separator();
        }

        ImGui::PopID();
    }

    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    ImGui::PopStyleVar();

    m_ViewportWidth = ImGui::GetContentRegionAvail().x;
    m_ViewportHeight = ImGui::GetContentRegionAvail().y;

    if (m_Renderer.GetFinalImage())
        ImGui::Image(
            (uint32_t*)m_Renderer.GetFinalImage()->GetRendererID(),
            {(float)m_Renderer.GetFinalImage()->GetWidth(), (float)m_Renderer.GetFinalImage()->GetHeight()},
            ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}

void Screen::Render()
{
    Timer timer;

    if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
    {
        m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
        m_Renderer.Render(m_Scene, m_Camera);
    }
    
    m_LastRenderTime = timer.ElapsedMillis();
}
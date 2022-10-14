#include "Screen.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"
#include "Application.h"

Screen::Screen()
    : m_Camera(60.0f, 0.1f, 100.0f)
{
    {
        Sphere sphere;
        sphere.Position = { 0.0f, 0.0f, -5.0f };
        sphere.Radius = 0.5f;
        sphere.Albedo = { 1.0f, 0.0f, 1.0f };
        m_Scene.Spheres.push_back(sphere);
    }
    {
        Sphere sphere;
        sphere.Position = { 0.0f, -100.5f, -5.0f };
        sphere.Radius = 100.0f;
        sphere.Albedo = { 0.0f, 1.0f, 1.0f };
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
    
    ImGui::Begin("Scene");
    if (ImGui::Button("Add Sphere +"))
        m_Scene.Spheres.push_back(Sphere());

    for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
    {
        ImGui::PushID(i);

        Sphere& sphere = m_Scene.Spheres[i];
        if (ImGui::TreeNode("Sphere"))
        {
            ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
            ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
            ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.Albedo), 0.1f);
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
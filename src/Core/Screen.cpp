#include "Screen.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Timer.h"
#include "Application.h"

void Screen::OnUIRender()
{
    m_Camera.OnUpdate(Application::Get().GetDeltaTime());
    Render();
    ImGui::Begin("Settings");
    ImGui::Text("Last render: %.3fms", m_LastRenderTime);
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
        m_Renderer.Render(m_Camera);
    }
    
    m_LastRenderTime = timer.ElapsedMillis();
}
#include "Screen.h"

#include <stdlib.h>
#include <time.h>

#include "imgui.h"

#include "Timer.h"

void Screen::OnUIRender()
{
    ImGui::Begin("Settings");
    ImGui::Text("Last render: %.3fms", m_LastRenderTime);
    if (ImGui::Button("Render"))
    {
        Render();
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Viewport");
    ImGui::PopStyleVar();

    m_ViewportWidth = ImGui::GetContentRegionAvail().x;
    m_ViewportHeight = ImGui::GetContentRegionAvail().y;

    if (m_Image)
        ImGui::Image((uint32_t*)m_Image->GetRendererID(), {(float)m_Image->GetWidth(), (float)m_Image->GetHeight()});

    ImGui::End();
    Render();
}

void Screen::Render()
{
    Timer timer;

    if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight())
    {
        m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, nullptr);
        delete[] m_ImageData;

        m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
        srand(time(NULL));
    }

    for (uint32_t i = 0; i < m_ViewportWidth * m_ViewportHeight; i++)
    {
        m_ImageData[i] = rand() % 0xffffffff;
        m_ImageData[i] |= 0xff000000;
    }

    m_Image->SetData(m_ImageData);
    m_LastRenderTime = timer.ElapsedMillis();
}
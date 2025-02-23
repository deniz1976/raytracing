#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);

		if (ImGui::Button("Render")) {
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		if(m_Image)
		{
		ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(),(float)m_Image->GetHeight() });
		}

		ImGui::End();
		ImGui::PopStyleVar();
		Render();
		
		//ImGui::ShowDemoWindow();
	}

	void Render()
	{
		Timer timer;

		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight()) 
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth,m_ViewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportHeight * m_ViewportWidth];
		}

		for(uint32_t i = 0 ; i < m_ViewportHeight * m_ViewportWidth ; i++)
		{
			m_ImageData[i] = Random::UInt();
			m_ImageData[i] |= 0xff000000;
		}

		m_Image->SetData(m_ImageData);
		m_LastRenderTime = timer.ElapsedMillis();

	}
private:
	std::shared_ptr<Image> m_Image;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	uint32_t* m_ImageData = nullptr;
	float m_LastRenderTime = 0.0f;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}
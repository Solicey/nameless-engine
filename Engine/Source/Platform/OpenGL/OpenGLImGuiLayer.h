#pragma once

#include "ImGui/ImGuiLayer.h"

namespace NL
{
	class OpenGLImGuiLayer : public ImGuiLayer
	{
	public:
		OpenGLImGuiLayer() = default;
		OpenGLImGuiLayer(const std::string& name) : ImGuiLayer(name) {}
		~OpenGLImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		virtual void Begin() override;
		virtual void End() override;

	private:
		float m_Time = 0.0f;
	};
}
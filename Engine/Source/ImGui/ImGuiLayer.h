#pragma once

#include "Core/Layer/Layer.h"
#include "Core/Misc/PtrWrapper.h"
#include "Events/Event.h"

namespace NL
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		ImGuiLayer(const std::string& name) : Layer(name) {}
		~ImGuiLayer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event& event) override;
		virtual void Begin() = 0;
		virtual void End() = 0;

		static Ref<ImGuiLayer> Create();

		/*
		* 当Viewport窗口不处于Focused或Hovered状态时，阻塞事件
		*/
		void BlockEvents(bool block) { m_BlockEvents = block; }

	private:
		bool m_BlockEvents = true;
	};
}
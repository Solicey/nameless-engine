#pragma once

#include <Engine.h>

namespace NL
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer() : Layer("EditorLayer") {}
		virtual ~EditorLayer() = default;

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate(TimeStep ts);
		virtual void OnImGuiRender();
		virtual void OnEvent(Event& event);
	};
}
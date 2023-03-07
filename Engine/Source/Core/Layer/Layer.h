#pragma once

#include "Events/Event.h"
#include "Core/Misc/TimeStep.h"

namespace NL
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer") {}
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}



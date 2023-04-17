#pragma once

#include "Core/Misc/Singleton.h"
#include "Events/Event.h"

namespace NL
{
	class ScriptGlue : public Singleton<ScriptGlue>
	{
	public:
		ScriptGlue(token) {}
		static void RegisterComponents();
		static void RegisterFunctions();

		using EventCallbackFn = std::function<void(Event&)>;

		void SetEventCallback(const EventCallbackFn& callback);
		void CallEventCallback(Event& event) { s_EventCallback(event); }

	private:
		EventCallbackFn s_EventCallback;
	};
}
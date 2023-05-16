#pragma once

#include "ECS/Component/Component.h"
#include "Camera/Camera.h"

namespace NL
{
	enum class AntiAliasingType : int
	{
		None, MSAA
	};
	
	enum class RenderMode
	{
		Forward, Deferred
	};

	// When add new settings: modify serializer and editor layer UI
	class SettingsComponent : public ComponentBase
	{
	public:

	public:
		int MSAASamples = 4;
		AntiAliasingType AntiAliasingType = AntiAliasingType::MSAA;
		Camera::ClearFlagType EditorCameraClearFlag = Camera::ClearFlagType::Color;
		bool ShowGizmos = true;
		RenderMode RenderMode = RenderMode::Forward;
	};
}
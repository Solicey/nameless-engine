#pragma once

#include "ECS/Component/Component.h"
#include "Camera/Camera.h"

namespace NL
{
	enum class AntiAliasingType : int
	{
		None, MSAA
	};

	class SettingsComponent : public ComponentBase
	{
	public:

	public:
		int MSAASamples = 4;
		AntiAliasingType AntiAliasingType = AntiAliasingType::MSAA;
		Camera::ClearFlagType EditorCameraClearFlag = Camera::ClearFlagType::Color;
		bool ShowGizmos = true;
	};
}
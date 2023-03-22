/*
Implement AppInitialize here.
*/

#include "Layer/EditorLayer.h"

#include <Engine.h>

#include <imgui.h>

namespace NL
{
	void AppInitialize(Application& app)
	{
		app.Init("Nameless Editor - New Scene");
		app.PushLayer(CreateRef<EditorLayer>());
	}
}

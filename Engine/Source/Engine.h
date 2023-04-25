#pragma once

#include "Core/App/Application.h"
#include "Core/Layer/Layer.h"
#include "Core/Layer/LayerStack.h"
#include "Core/Log/Log.h"
#include "Core/Math/Math.h"
#include "Core/Misc/TimeStep.h"
#include "Core/Misc/PtrWrapper.h"
#include "Core/Misc/Directives.h"
#include "Core/Misc/PathConfig.h"
#include "Core/Misc/Singleton.h"

#include "Camera/Camera.h"
#include "Camera/EditorCamera.h"

#include "ECS/Component/AllComponents.h"
#include "ECS/System/AllSystems.h"
#include "ECS/Entity/Entity.h"
#include "ECS/Scene/Scene.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/MiscEvent.h"

#include "Input/Input.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

#include "ImGui/ImGuiLayer.h"

#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "Resources/Libraries/ShaderLibrary.h"
#include "Resources/Libraries/TextureLibrary.h"
#include "Resources/Loaders/ModelLoader.h"
#include "Resources/Loaders/ModelLoaderFlags.h"
#include "Resources/Serializer/SceneSerializer.h"

#include "Renderer/Renderer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/PostProcessing.h"

#include "Scripting/ScriptEngine.h"
#include "Scripting/ScriptGlue.h"

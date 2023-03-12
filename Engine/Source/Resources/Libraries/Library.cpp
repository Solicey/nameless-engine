#include "nlpch.h"

#include "Library.h"

#include "Resources/Loaders/ModelLoader.h"

namespace NL
{
	template<typename LibType>
	Library<LibType>::Library()
	{
	}

	template<>
	Library<Model>::Library()
	{
		// ?
		Add("Box", ModelLoader::Create("E:/dev/nameless-engine/Engine/Resources/Models/Box.obj", ModelLoaderFlags::Triangulate));
	}
}
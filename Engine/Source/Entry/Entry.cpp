#include "nlpch.h"
#include "Core/App/Application.h"

namespace NL
{
	// Defined in APP.
	extern void AppInitialize(Application& app);
}

int main(int argc, char** argv)
{
	NL::AppInitialize(NL::Application::GetInstance());
	NL::Application::GetInstance().Run();
	NL::Application::GetInstance().Clean();
}
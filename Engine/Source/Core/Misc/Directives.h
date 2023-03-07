#pragma once

/*
‘§¥¶¿Ì÷∏¡Ó
*/

#define BIT(x) (1 << x)

#ifdef NL_DEBUG
	#define NL_ENABLE_ASSERTS
#endif

#ifdef NL_ENABLE_ASSERTS
	#define NL_ASSERT(x, ...) { if(!(x)) {NL_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak(); }}
	#define NL_ENGINE_ASSERT(x, ...) { if(!(x)) {NL_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak(); }}
#else
	#define NL_ASSERT(x, ...)
	#define NL_ENGINE_ASSERT(x, ...)
#endif
#pragma once

/*
预处理指令
*/

#define BIT(x) (1 << x)

#ifdef NL_DEBUG
	#define NL_ENABLE_ASSERTS
#endif

// 适用于非静态成员函数绑定
#define NL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
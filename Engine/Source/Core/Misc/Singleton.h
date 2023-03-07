#pragma once

/*
单例模式
*/

#include <type_traits>

namespace NL
{
	// 奇异递归模板模式下的单例类模板
	template<typename T>
	class Singleton
	{
	public:
		// is_nothrow_constructible: 检查给定类型T是否是带有参数集的可构造类型
		static T& GetInstance() noexcept(std::is_nothrow_constructible<T>::value)	
		{
			static T instance{ token() };
			return instance;
		}

		virtual ~Singleton() = default;

		Singleton(const Singleton&) = delete;
		Singleton& operator = (const Singleton&) = delete;

	protected:
		struct token{};	// 代理类令牌
		Singleton() noexcept = default;
	};
}
#pragma once

/*
����ģʽ
*/

#include <type_traits>

namespace NL
{
	// ����ݹ�ģ��ģʽ�µĵ�����ģ��
	template<typename T>
	class Singleton
	{
	public:
		// is_nothrow_constructible: ����������T�Ƿ��Ǵ��в������Ŀɹ�������
		static T& GetInstance() noexcept(std::is_nothrow_constructible<T>::value)	
		{
			static T instance{ token() };
			return instance;
		}

		virtual ~Singleton() = default;

		Singleton(const Singleton&) = delete;
		Singleton& operator = (const Singleton&) = delete;

	protected:
		struct token{};	// ����������
		Singleton() noexcept = default;
	};
}
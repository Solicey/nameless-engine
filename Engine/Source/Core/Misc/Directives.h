#pragma once

/*
Ԥ����ָ��
*/

#define BIT(x) (1 << x)

#ifdef NL_DEBUG
	#define NL_ENABLE_ASSERTS
#endif

// �����ڷǾ�̬��Ա������
#define NL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
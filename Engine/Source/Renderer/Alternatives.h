#pragma once

namespace NL
{
	enum class TextureFilteringMode
	{
		Nearest, 
		Linear, 
		NearestMipmapNearest,	// ʹ�����ڽ��Ķ༶��Զ������ƥ�����ش�С����ʹ���ڽ���ֵ�����������
		LinearMipmapNearest,	// ʹ�����ڽ��Ķ༶��Զ�����𣬲�ʹ�����Բ�ֵ���в���
		NearestMipmapLinear,	// 	��������ƥ�����ش�С�Ķ༶��Զ����֮��������Բ�ֵ��ʹ���ڽ���ֵ���в���
		LinearMipmapLinear		// �������ڽ��Ķ༶��Զ����֮��ʹ�����Բ�ֵ����ʹ�����Բ�ֵ���в���
	};
}
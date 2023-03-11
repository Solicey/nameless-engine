#pragma once

namespace NL
{
	enum class TextureFilteringMode
	{
		Nearest, 
		Linear, 
		NearestMipmapNearest,	// 使用最邻近的多级渐远纹理来匹配像素大小，并使用邻近插值进行纹理采样
		LinearMipmapNearest,	// 使用最邻近的多级渐远纹理级别，并使用线性插值进行采样
		NearestMipmapLinear,	// 	在两个最匹配像素大小的多级渐远纹理之间进行线性插值，使用邻近插值进行采样
		LinearMipmapLinear		// 在两个邻近的多级渐远纹理之间使用线性插值，并使用线性插值进行采样
	};
}
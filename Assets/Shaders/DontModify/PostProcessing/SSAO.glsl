// Gray scale shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

#use post
#tag ssao;ssao;

#prop
int u_KernelSize;
float u_Radius;
#end

#type vertex
#version 450 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;

layout (location = 0) out vec2 v_TexCoords;
			
void main()
{
	v_TexCoords = a_TexCoords;
    gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0); 
}

#type fragment
#version 450 core

layout (location = 0) in vec2 v_TexCoords;

layout (location = 0) out vec4 color;

uniform sampler2D u_PositionDepthTex;
uniform sampler2D u_NormalTex;

uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

// tag ssao;
uniform sampler2D u_NoiseTex;
uniform vec3 u_Samples[64];

uniform float u_Radius;
uniform int u_KernelSize;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};
	
void main()
{
	vec2 noiseScale = vec2(u_ScreenWidth / 4.0, u_ScreenHeight / 4.0);

	vec3 fragPos = texture(u_PositionDepthTex, v_TexCoords).xyz;
	float depth = texture(u_PositionDepthTex, v_TexCoords).w;
	vec3 normal = normalize(texture(u_NormalTex, v_TexCoords).xyz);
	vec3 randomVec = texture(u_NoiseTex, v_TexCoords * noiseScale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	
	float occlusion = 0.0;
	float tot = 0;
	for (int i = 0; i < min(u_KernelSize, 64); i++)
	{
		vec3 samp = TBN * u_Samples[i]; // 切线->观察空间
		samp = fragPos + samp * u_Radius; // radius 
		vec4 offset = vec4(samp, 1.0);
		offset = u_Projection * offset; // 观察->裁剪空间
		offset.xyz /= offset.w; // 透视除法
		offset.xyz = offset.xyz * 0.5 + 0.5;
		float sampleDepth = -texture(u_PositionDepthTex, offset.xy).w;
		//vec3 tmp = texture(u_NormalTex, offset.xy).xyz;
		if (sampleDepth < 0)
		{
			float rangeCheck = smoothstep(0.0, 1.0, u_Radius / abs(fragPos.z - sampleDepth));
			occlusion += (sampleDepth >= samp.z ? 1.0 : 0.0) * rangeCheck;
			tot += 1.0;
		}
	}
	occlusion = 1.0 - (occlusion / u_KernelSize);
	tot = tot / u_KernelSize;

	color = vec4(vec3(occlusion), 1);

	//color = vec4(vec3(avg / 64.0), 1);
	//color = vec4(normal, 1.0);
}			
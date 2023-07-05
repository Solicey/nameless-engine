// Gray scale shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

#use post

// Require ssao noise tex and src color tex
#tag ssao;src;

#prop
int u_KernelSize;
float u_Radius;
float u_Threshold;
color3 u_AmbientColor;
#end

// u_Threshold = 0.05

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
uniform sampler2D u_SrcColorTex;

uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

// tag ssao;
#define SSAO_SAMPLE_COUNT 256
uniform sampler2D u_NoiseTex;
uniform vec3 u_Samples[SSAO_SAMPLE_COUNT];

uniform float u_Radius;
uniform int u_KernelSize;
uniform float u_Threshold;
uniform vec3 u_AmbientColor;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

float random(float x)
{
    float y = fract(sin(x)*100000.0);
    return y;
}
	
void main()
{
	vec2 noiseScale = vec2(u_ScreenWidth / 4.0, u_ScreenHeight / 4.0);

	vec3 fragPos = texture(u_PositionDepthTex, v_TexCoords).xyz;
	float depth = texture(u_PositionDepthTex, v_TexCoords).w;
	vec3 normal = normalize(texture(u_NormalTex, v_TexCoords).rgb);
	vec3 randomVec = texture(u_NoiseTex, v_TexCoords * noiseScale).xyz;
	//randomVec = normalize(vec3(random(normal.x), random(v_TexCoords.y), random(fragPos.x)));
	//randomVec = vec3(1, 1, 1);
	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
	
	float occlusion = 0.0;
	vec3 indirLight = vec3(0, 0, 0);
	vec3 dirLight = vec3(0, 0, 0);
	float validCount = 0;

	for (int i = 0; i < min(u_KernelSize, SSAO_SAMPLE_COUNT); i++)
	{
		vec3 samp = TBN * u_Samples[i]; // 切线->观察空间

		if (dot(samp, normal) < u_Threshold)	// Avoid artifact
			continue;
		validCount = validCount + 1.0;

		samp = fragPos + samp * u_Radius; 
		vec4 offset = vec4(samp, 1.0);
		offset = u_Projection * offset; // 观察->裁剪空间
		offset.xyz /= offset.w; // 透视除法
		offset.xyz = offset.xyz * 0.5 + 0.5;
		
		vec3 samplePos = texture(u_PositionDepthTex, offset.xy).xyz;
		float sampleDepth = -texture(u_PositionDepthTex, offset.xy).w;
		vec3 sampleNormal = texture(u_NormalTex, offset.xy).xyz;
		vec3 sampleColor = texture(u_SrcColorTex, offset.xy).rgb;

		//vec3 tmp = texture(u_NormalTex, offset.xy).xyz;
		if (sampleDepth < 0)
		{
			float rangeCheck = smoothstep(0.0, 1.0, u_Radius / abs(fragPos.z - sampleDepth));
			float depthDiff = sampleDepth - samp.z;

			if (depthDiff > 0.0)
			{
				indirLight += rangeCheck * sampleColor * max(dot(sampleNormal, normalize(fragPos - samplePos)), 0.0);
				occlusion += ((depthDiff > 0.0) ? 1.0 : 0.0) * rangeCheck;
			}
			else
			{
				vec3 skyboxColor = vec3(0.1, 0.1, 0.1);
				dirLight += rangeCheck * skyboxColor * max(dot(normal, normalize(samp - fragPos)), 0.0);
			}
		}
	}
	occlusion = 1.0 - (occlusion / validCount);
	indirLight = indirLight / validCount;
	dirLight = dirLight / validCount;

	color = vec4((indirLight) * 3 + dirLight, 1);

	//color = vec4(vec3(avg / 64.0), 1);
	//color = vec4(normal, 1.0);
}			
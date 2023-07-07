// Gray scale shader
// For post-processing only, don't use it on entities
// You don't need to expose any properties

#use post

// Require ssao noise tex and src color tex
#tag ssao;src;skybox;

#prop
int u_KernelSize;
float u_Radius;
float u_Threshold;
float u_EnableSRGBCorrection;
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
uniform samplerCube u_Skybox;

uniform int u_ScreenWidth;
uniform int u_ScreenHeight;

// tag ssao;
#define SSAO_SAMPLE_COUNT 256
uniform sampler2D u_NoiseTex;
uniform vec3 u_Samples[SSAO_SAMPLE_COUNT];

uniform float u_Radius;
uniform int u_KernelSize;
uniform float u_Threshold;
uniform float u_EnableSRGBCorrection;

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
	vec3 fragColor = texture(u_SrcColorTex, v_TexCoords).rgb;
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

	mat4 invView = inverse(u_View);

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
		float depthDiff = sampleDepth - samp.z;

		if (sampleDepth < 0 && depthDiff > 0.0)
		{
			float rangeCheck = smoothstep(0.0, 1.0, u_Radius / abs(fragPos.z - sampleDepth));
			indirLight += rangeCheck * sampleColor * max(dot(sampleNormal, normalize(fragPos - samplePos)), 0.0);
			occlusion += rangeCheck;
		}
		else
		{
			vec4 dir = invView * vec4(samp - fragPos, 0.0);
			vec3 skyboxColor = texture(u_Skybox, dir.xyz).xyz;
			if (u_EnableSRGBCorrection != 0)
			{
				skyboxColor = pow(skyboxColor, vec3(2.2));
			}
			dirLight += skyboxColor * max(dot(normal, normalize(samp - fragPos)), 0.0);
		}
	}
	occlusion = 1.0 - (occlusion / validCount);
	indirLight = indirLight / validCount;
	dirLight = dirLight / validCount;

	color = vec4(indirLight + dirLight, 1);

	//color = vec4(vec3(avg / 64.0), 1);
	//color = vec4(vec3(depth), 1.0);
	//color = vec4(fragColor, 1.0);
}			
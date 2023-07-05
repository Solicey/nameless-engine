// nlsl template shader file

#use model
#lit 4

#prop

sampler2D u_Diffuse;
sampler2D u_Metalness;
sampler2D u_Normals;
sampler2D u_Roughness;
float u_IsNormalsCompressed;

#end

// You MUST expose all of your sampler2D up here
// sampler2D must have a correct name

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

uniform mat4 u_Transform;
uniform mat3 u_NormalMatrixWS;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out mat3 v_TBN;
			
void main()
{
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	v_TexCoord = a_TexCoord;
	v_Normal = normalize(u_NormalMatrixWS * a_Normal);
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);

	vec3 T = normalize(u_NormalMatrixWS * a_Tangent);
	vec3 B = normalize(u_NormalMatrixWS * a_Bitangent);
	vec3 N = normalize(u_NormalMatrixWS * a_Normal);
	v_TBN = mat3(T, B, N);
}

// FRAGMENT

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in mat3 v_TBN;

layout (location = 0) out vec4 f_Color;
layout (location = 1) out int f_EntityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

uniform int u_EntityId;

uniform sampler2D u_Diffuse;
uniform sampler2D u_Metalness;
uniform sampler2D u_Normals;
uniform sampler2D u_Roughness;
uniform float u_IsNormalsCompressed;

#define MAX_LIGHT_COUNT 4
const float PI = 3.14159265359;

struct PointLight
{
	vec3 Position;
	vec3 Color;
	vec3 Attenuation;
};
uniform PointLight u_PointLights[MAX_LIGHT_COUNT];

struct DirLight
{
	vec3 Direction;
	vec3 Color;
};
uniform DirLight u_DirLights[MAX_LIGHT_COUNT];

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
			
void main()
{
	vec3 albedo = texture(u_Diffuse, v_TexCoord).rgb;
	albedo.r = pow(albedo.r, 2.2);
	albedo.g = pow(albedo.g, 2.2);
	albedo.b = pow(albedo.b, 2.2);
	
	vec3 normal = texture(u_Normals, v_TexCoord).rgb;
	// For compressed normal map
	if (u_IsNormalsCompressed != 0)
		normal.z = sqrt(1 - normal.x * normal.x - normal.y * normal.y);
	normal = normalize(normal * 2.0 - vec3(1.0));
	normal = normalize(v_TBN * normal);

	float metallic = texture(u_Metalness, v_TexCoord).r;
	float roughness = texture(u_Roughness, v_TexCoord).r;

	vec3 V = normalize(u_CameraPosition - v_Position);
	vec3 N = normal;
	vec3 worldPos = v_Position;
	vec3 Lo = vec3(0.0);
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		// PointLights
		if (u_PointLights[i].Color.r >= 0)
		{
			vec3 lightPos = u_PointLights[i].Position;
			vec3 lightColor = u_PointLights[i].Color;
			vec3 lightAtten = u_PointLights[i].Attenuation;

			vec3 L = normalize(lightPos - worldPos);
			vec3 H = normalize(V + L);

			float dist = length(lightPos - worldPos);
			// float atten = 1.0 / (lightAtten.x + lightAtten.y * dist + lightAtten.z * dist * dist);
			float atten = 1.0 / (dist * dist);
			vec3 radiance = lightColor * atten;

			vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
			float NDF = DistributionGGX(N, H, roughness);
			float G = GeometrySmith(N, V, L, roughness);

			vec3 nominator = NDF * G * F;
			float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
			vec3 specular = nominator / denominator;

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;

			float NdotL = max(dot(N, L), 0.0);
			Lo += (kD * albedo / PI + specular) * radiance * NdotL;
		}
	}

	vec3 ambient = vec3(0.03) * albedo;
	vec3 color = ambient + Lo;

	// Gamma Correction
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));
	f_Color = vec4(color, 1.0);

	// Dont Modify
	f_EntityId = u_EntityId;
}			
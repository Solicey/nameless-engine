// nlsl template shader file

#use model

#prop

color3 u_Color;
float u_SpecularStrength;
sampler2D u_Diffuse;
sampler2D u_Specular;
sampler2D u_Normals;

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
// Normal Matrix
uniform mat4 u_NormalMatrix;

layout (location = 0) out vec3 v_Position;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out mat3 v_TBN;
			
void main()
{
	v_Position = vec3(u_View * u_Transform * vec4(a_Position, 1.0));
	v_TexCoord = a_TexCoord;
	v_Normal = a_Normal;
	gl_Position = u_Projection * u_View * u_Transform * vec4(a_Position, 1.0);

	vec3 T = vec3(normalize(u_NormalMatrix * vec4(a_Tangent, 0)));
	vec3 B = vec3(normalize(u_NormalMatrix * vec4(a_Bitangent, 0)));
	vec3 N = vec3(normalize(u_NormalMatrix * vec4(a_Normal, 0)));
	v_TBN = mat3(T, B, N);
}

// FRAGMENT

#type fragment
#version 450 core

layout (location = 0) in vec3 v_Position;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in mat3 v_TBN;

layout (location = 0) out vec4 f_AlbedoSpec;
layout (location = 1) out int f_EntityId;
layout (location = 2) out vec4 f_Position;
layout (location = 3) out vec4 f_Normal;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	vec3 u_CameraPosition;
	float u_Near;
	float u_Far;
};

float LinearizeDepth(float depth, float near, float far)
{
    float z = depth * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

// Object Color
uniform vec3 u_Color;
uniform int u_EntityId;
uniform float u_SpecularStrength;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Specular;
uniform sampler2D u_Normals;
uniform mat4 u_Transform;
	
void main()
{
	vec4 diffuseFactor = texture(u_Diffuse, v_TexCoord).rgba;
	if (diffuseFactor.a <= 0.05) discard;
	vec3 diffuseColor = diffuseFactor.rgb;

	vec4 specularFactor = texture(u_Specular, v_TexCoord).rgba;

	vec3 normal = texture(u_Normals, v_TexCoord).rgb;
	if (texture(u_Normals, v_TexCoord).a > 0)
	{
		normal = normalize(normal * 2.0 - vec3(1.0));
		normal = normalize(v_TBN * normal);
	}
	else
	{
		normal = v_Normal;
	}
	mat3 normalMatrix = transpose(inverse(mat3(u_View * u_Transform)));
	normal = normalize(normalMatrix * normal);
	// For compressed normal map
	// normal.z = sqrt(1 - normal.x * normal.x - normal.y * normal.y);
	

	f_AlbedoSpec.rgb = diffuseColor;
	f_AlbedoSpec.a = u_SpecularStrength * specularFactor.r;
	f_EntityId = u_EntityId;
	f_Position = vec4(v_Position, LinearizeDepth(gl_FragCoord.z, u_Near, u_Far));
	f_Normal = vec4(normal, 1.0);
}			